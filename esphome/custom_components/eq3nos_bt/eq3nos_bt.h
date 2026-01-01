#pragma once
#define DEBUG_YES
#include "eq3nos_connect.h"
#include "eq3nos_control.h"
#include "eq3_target_number.h"
#include "esphome/core/component.h"
#include "esphome/components/ble_client/ble_client.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"
#include "esphome/components/select/select.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"



namespace esphome {
namespace eq3nos_bt {

// Forward declaration 
class EQ3NOS;
class EQ3BoostSwitch;
class EQ3OffSwitch;
class EQ3ModeSelect;   
class EQ3LockSwitch;

// Mode selector
class EQ3ModeSelect : public select::Select, public Component {
 public:
  void set_parent(EQ3NOS *parent) { parent_ = parent; }

 protected:
  void control(const std::string &value) override;

  EQ3NOS *parent_{nullptr};
};

// LOCK switch
class EQ3LockSwitch : public esphome::switch_::Switch {
 public:

  void write_state(bool state) override;  
  void set_parent(EQ3NOS *parent) { parent_ = parent; }

 private:
  EQ3NOS *parent_{nullptr};
};

// BOOST Switch
class EQ3BoostSwitch : public esphome::switch_::Switch {
 public:

  void write_state(bool state) override;  // chiamato da Home Assistant
  void set_parent(EQ3NOS *parent) { parent_ = parent; }

 private:
  EQ3NOS *parent_{nullptr};
};

// OFF SWITCH
class EQ3OffSwitch : public esphome::switch_::Switch {
 public:
  void write_state(bool state) override;
  void set_parent(EQ3NOS *parent) { parent_ = parent; }

 private:
  EQ3NOS *parent_{nullptr};
};

// EQ3NOS Main
class EQ3NOS  
  : public ble_client::BLEClientNode, 
    public Component {

 public:      
		void setup() override;
		void loop() override;

        ble_client::BLEClient* ble() { return ble_client::BLEClientNode::parent_; } 
        
        // Override BLE client event
		void gattc_event_handler(esp_gattc_cb_event_t event,
			 esp_gatt_if_t gatt_if,
			 esp_ble_gattc_cb_param_t *param) override;

		void set_name(const std::string &name) { name_ = name; }
		void set_mac_address(const std::string &mac) { mac_address_ = mac; }
		

        text_sensor::TextSensor *eq3_serial_sensor_{nullptr};
		void set_serial_sensor(text_sensor::TextSensor *s) { eq3_serial_sensor_ = s; }

		text_sensor::TextSensor *eq3_firmware_sensor_{nullptr};
        void set_firmware_sensor(text_sensor::TextSensor *s) { eq3_firmware_sensor_ = s; }

        binary_sensor::BinarySensor *watchdog_sensor_{nullptr};
        void set_watchdog_sensor(binary_sensor::BinarySensor *sens) { watchdog_sensor_ = sens; }

		sensor::Sensor *valve_position_sensor_{nullptr};
        void set_valve_position_sensor(sensor::Sensor *sensor) { valve_position_sensor_ = sensor;}

		sensor::Sensor *current_temperature_sensor_{nullptr};
        void set_current_temperature_sensor(sensor::Sensor *sensor) {current_temperature_sensor_ = sensor;}	

		sensor::Sensor *target_temperature_sensor_{nullptr};
        void set_target_temperature_sensor(sensor::Sensor *sensor) { target_temperature_sensor_ = sensor;}

		binary_sensor::BinarySensor *battery_low_sensor_{nullptr};
        void set_battery_low_sensor(binary_sensor::BinarySensor *sensor) { battery_low_sensor_ = sensor;}
        
        sensor::Sensor *recovery_counter_sensor_{nullptr};      
        void set_recovery_counter_sensor(sensor::Sensor *sens) { recovery_counter_sensor_ = sens; }

        void set_target_temperature(float temp);
		void set_target_number(EQ3TargetNumber *num) {
		    target_number_ = num;
            num->set_parent(this);
        }
		
        void set_mode_select(EQ3ModeSelect *sel);
		void set_lock_switch( EQ3LockSwitch *sw) { lock_switch_ = sw; }
		void set_boost_switch(EQ3BoostSwitch *sw) { boost_switch_ = sw; }
		void set_off_switch(EQ3OffSwitch *sw) { off_switch_ = sw; }
        
	    void publish_eq3_identification( std::string &sn,  std::string &fw);
		void publish_base_field_(uint8_t vp, float tt, Eq3Mode mode, bool lm, bool bt, bool ws, bool bl);
        void publish_recovery_counter_sensor(uint16_t rc_);
       
       	void send_command_to_connection(const ConnCommand &cmd);
		void send_msg_to_app(const std::vector<uint8_t> &msg);
			
		// switch
		void request_boost(bool state);
		void request_off(bool state);

		
		EQ3Control control_;
		
		void set_time_source(esphome::time::RealTimeClock *t) {this->time_source_ = t; }
		esphome::time::RealTimeClock *time_source_{nullptr};
		esphome::ESPTime get_eq3_time();
		
	
protected:
		std::string name_;
		std::string mac_address_;
		std::string serial_number_;
		std::string eq3_firmware_;
		uint8_t valve_percent;
		float current_temperature;
		float target_temperature;

        bool watchdog_state_{false};
        uint32_t recovery_counter_{0};
		Eq3Mode mode_status;
        bool lock_mode;
		bool windows;
		bool boost;
		bool battery_low;
		float setpoint_;
		
        EQ3ModeSelect *mode_select_{nullptr};
        EQ3LockSwitch *lock_switch_{nullptr};
		EQ3TargetNumber *target_number_{nullptr};
		EQ3BoostSwitch *boost_switch_{nullptr};
		EQ3OffSwitch *off_switch_{nullptr};

		float target_temperature_{20.0};
        void module_init();
       

private:
		EQ3Connection connection_;
        bool module_to_init = true;
        bool sync_target_temperature = true;
        bool ble_client_init_error = false;
   
        
};

}  // namespace eq3nos_bt
}  // namespace esphome

