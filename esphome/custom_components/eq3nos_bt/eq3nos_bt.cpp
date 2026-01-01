#include "eq3nos_bt.h"
#include "esphome/core/log.h"
#include "eq3nos_conncommand.h"

namespace esphome {
namespace eq3nos_bt {

static const char *TAG = "eq3nos_bt";

/*
* Mode Selector
*/
void EQ3ModeSelect::control(const std::string &value) {

    if (!parent_) {
        ESP_LOGW(TAG, "Mode select parent not set!");
        return;
    }
    if (value == "auto") {
        parent_->control_.cmd_set_mode(Eq3Mode::AUTO);
    } else if (value == "manual") {
        parent_->control_.cmd_set_mode(Eq3Mode::MANUAL);
    } else if (value == "vacation") {
        parent_->control_.cmd_set_mode(Eq3Mode::VACATION);
    }
    //mode_select_->publish_state(value);
}

/*
* LOCK switch
*/
void EQ3LockSwitch::write_state(bool state) {
    if (!parent_) {
        ESP_LOGW(TAG, "Lock switch parent not set!");
        return;
    }
   parent_->control_.cmd_lock(state);
   this->publish_state(state);
}

// =================== BOOST SWITCH ===================
void EQ3BoostSwitch::write_state(bool state) {
   // ESP_LOGI(TAG, "Boost switch changed state: %s", state ? "ON" : "OFF");
    if (!parent_) {
        ESP_LOGW(TAG, "Boost switch parent not set!");
        return;
    }
    parent_->control_.cmd_boost(state);
    this->publish_state(state);
}

// =================== OFF SWITCH ===================
void EQ3OffSwitch::write_state(bool state) {
  ESP_LOGI(TAG, "Off switch changed state: %s", state ? "ON" : "OFF");
  if (!parent_) {
    ESP_LOGW(TAG, "Off switch parent not set!");
    return;
  }

  //parent_->request_off(state);
}

void EQ3NOS::set_mode_select(EQ3ModeSelect *sel) {
   ESP_LOGW(TAG, "I2NOS - Set mode select");
  mode_select_ = sel;
}

void EQ3NOS::request_boost(bool state) {
  ESP_LOGI(TAG, " Cosa devo fare qui? Request BOOST state: %s", state ? "ON" : "OFF");

}

/*
* Set target temperatura - Richiamata da Home Assistant
*/
void EQ3NOS::set_target_temperature(float temp) {
    temp = clamp(temp, 5.0f, 30.0f);
    uint8_t target = static_cast<uint8_t>(roundf(temp * 2.0f));
    control_.send_setpoint_(target);
    ESP_LOGI("EQ3", "Setpoint %.1f °C -> TT = 0x%02X (%d)",
         temp, target, target);
}

/*
* Callback eventi GATT (override diretto da BLEClientNode)
*/
void EQ3NOS::gattc_event_handler  (esp_gattc_cb_event_t event,
                                esp_gatt_if_t gatt_if,
                                esp_ble_gattc_cb_param_t *param) {
    connection_.on_ble_event(event, gatt_if, param);
}

/*
* Pubblica serial number e fimware version
*/
void EQ3NOS::publish_eq3_identification(std::string &sn, std::string &fw) {
    this->serial_number_ = sn;
    this->eq3_firmware_ = fw;
	
   if (eq3_serial_sensor_ != nullptr)
        eq3_serial_sensor_->publish_state(this->serial_number_);
   if (eq3_firmware_sensor_ != nullptr)
        eq3_firmware_sensor_->publish_state(this->eq3_firmware_);
}        

/*
* Pubblica il recovery counter
*/
void EQ3NOS::publish_recovery_counter_sensor(uint16_t rc_) {
    if (recovery_counter_sensor_ != nullptr) {
        recovery_counter_sensor_->publish_state(rc_);
    }
}

/*
* Pubblica temperature e flags
*/
void EQ3NOS::publish_base_field_(uint8_t vp, float tt,  Eq3Mode mode, bool lm, bool bt, bool ws, bool bl){
    this->valve_percent = vp;
    //this->current_temperature = ct;
    this->target_temperature = tt;
    this->mode_status = mode;
    this->lock_mode = lm;
    this->boost = bt;
    this->windows = ws;
	this->battery_low = bl;

    if (mode_select_ != nullptr) {
        switch (mode) {
            case Eq3Mode::AUTO:
                mode_select_->publish_state("auto");
            break;
            case Eq3Mode::MANUAL:
                mode_select_->publish_state("manual");
            break;
            case Eq3Mode::VACATION:
                mode_select_->publish_state("vacation");
            break;
        }
    }

    if (valve_position_sensor_ != nullptr) {
        valve_position_sensor_->publish_state( this->valve_percent);
    } 
    if (target_temperature_sensor_ != nullptr) {
        target_temperature_sensor_->publish_state(this->target_temperature);
    }   
  //  if (current_temperature_sensor_ != nullptr) {
  //      current_temperature_sensor_->publish_state(this->current_temperature);
  //  }

    if (lock_switch_ != nullptr) {
        lock_switch_->publish_state(this->lock_mode);
    }

    if (boost_switch_ != nullptr) {
        boost_switch_->publish_state(bt);
    }

    if (battery_low_sensor_ != nullptr) {
        battery_low_sensor_->publish_state(this->battery_low);
    }
    
    if(sync_target_temperature){
        if (target_number_ != nullptr) {
        sync_target_temperature = false;
        target_number_->publish_state(this->target_temperature);
        }
    }

    watchdog_state_ = !watchdog_state_;
    if (watchdog_sensor_ != nullptr) {
        watchdog_sensor_->publish_state(watchdog_state_);
    }
}

/*
* Ruota comando al task connection
*/
void EQ3NOS::send_command_to_connection(const ConnCommand &cmd ) {  
		this->get_eq3_time();
    bool error = connection_.connect_queuer(cmd);
    //ESP_LOGI(TAG, "Accodamento a connect riuscito: %s", error ? "SI" : "NO");   
}

/*
* Ruota il messaggio all'applicazione
*/
void EQ3NOS::send_msg_to_app(const std::vector<uint8_t> &msg) {
    this->control_.handle_incoming_message(msg);
}

esphome::ESPTime EQ3NOS::get_eq3_time(){
	auto now = time_source_->now();
	ESP_LOGI("eq3", "Time now: %04d-%02d-%02d %02d:%02d:%02d",
         now.year, now.month, now.day_of_month,
         now.hour, now.minute, now.second);
         return now;
}


// =================== EQ3NOS ===================
void EQ3NOS::setup() {

    this->ble_client_init_error = true;
    if (ble_client::BLEClientNode::parent_ == nullptr) {
        this->ble_client_init_error = true;
    } else 
        this->ble_client_init_error = false;

    // Registro il nodo sul BLE client
    this->set_ble_client_parent(ble_client::BLEClientNode::parent_);
    ble_client::BLEClientNode::parent_->register_ble_node(this);

    control_.set_parent(this);
    control_.app_task_init();

    // Inizializzo connessione
    connection_.set_parent(this);      // parent per callback di connessione
    connection_.con_task_init();
    
    sync_target_temperature = true;  // sincronizza il display della temperatura target

    this->module_to_init = true;
}

void EQ3NOS::module_init() {
    //delay(3000);
    this->module_to_init = false;
    if (ble_client_init_error)
        ESP_LOGE(TAG, "I2NOS-->Parent BLE non impostato, setup interrotto");
    else
        ESP_LOGE(TAG, "I2NOS-->Parent BLE impostato correttamente");
     
    ESP_LOGI(TAG, "📡 EQ3NOS: setup completato"); 
}

/*
* Main loop
*/
void EQ3NOS::loop() {
    if(this->module_to_init == true)
        module_init();
    
    static uint32_t last_attempt = 0;
    
	if (millis() - last_attempt < 1000) return; // timer 1 secondo
    last_attempt = millis();
   
    this->control_.increment_timer_applic();
    this->connection_.increment_timer_connect();
    
    this->control_.app_task();
    this->connection_.connect_task();
}



}  // namespace eq3nos_bt
}  // namespace esphome

