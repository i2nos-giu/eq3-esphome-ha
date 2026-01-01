#pragma once

#include <cstdint>
#include "esphome/core/component.h"
#include "esphome/components/ble_client/ble_client.h"
#include "esphome/core/log.h"
#include <sstream>
#include <iomanip>
#include "Eq3Adapters.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/time/real_time_clock.h"


#define STATUSTIMER  3600
#define NORMALQUERY  300   // update info every 300 sec
#define RECOVERY_MAX_TIME 20 // after wait 10s reset busy

enum class Eq3Mode : uint8_t {
  AUTO = 0,
  MANUAL = 1,
  VACATION = 2,
};

namespace esphome {
namespace eq3nos_bt {

class EQ3NOS;   // forward declaration

enum class ApplicationStatus {
    APP_WAIT,
    GET_SERIAL_NUMBER,
    SET_MODE_AUTO,
    SET_MODE_MANUAL,
    SET_MODE_VACATION,
    SET_LOCK_SWITCH,
    NOW_UPDATE_TIME,
    GET_CURR_TEMPERATURE,
    SET_TARGET_TEMPERATURE,
    SET_BOOST_SWITCH,
    READ_STATUS
};

enum class AppCommandType {
    GET_INFO,
    UPDATE_TIME,
    SET_AUTO,
    SET_MANUAL,
    SET_VACATION,
    SET_LOCK,
    SET_TARGET_TEMP,
    SET_BOOST,
    GET_CURR_TEMP,
    READ_STATUS
};

#pragma pack(push, 1)
struct Eq3SetTime {
    uint8_t cmd;      // 03
    uint8_t year;     // year % 100
    uint8_t month;    // 1–12
    uint8_t day;      // 1–31
    uint8_t hour;     // 0–23
    uint8_t minute;   // 0–59
    uint8_t second;   // 0–59
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Eq3StatusResponse {
	uint8_t replaycode;       // [0] always 02
    uint8_t always1;          // [1] always 01
    uint8_t mode;             // [2]
    uint8_t valve_percent;    // [3] Valve position % (0–100)
    uint8_t unknown;  				// [4]
    uint8_t target_temp_x2;   // [5] Target Temp * 2 (es. 21.0°C → 42)
    uint8_t vm_day;           // [6] Vacantion mode day
    uint8_t vm_year ;         // [7] Vacantion mode year
    uint8_t vm_month;         // [8] Vacantion mode month
    uint8_t vm_time;          // [9] Vacantion mode time
    uint8_t temperature_ow;   // [10] temperature window-open
    uint8_t comfort_temperature;// [12] Temperature offset encoded
    uint8_t eco_temperature;  // [13] 
    uint8_t temperature_offset;// [14]
};
#pragma pack(pop)

struct ApplCommand {
    AppCommandType type;
    bool status;  // per eventuali comandi ON/OFF
    uint8_t value; // Per temperatura
};

class EQ3Control {
    public:
        void set_parent(EQ3NOS *parent);
        void app_task_init();

        void handle_incoming_message(const std::vector<uint8_t> &msg);

        bool applic_queuer(ApplCommand cmd);
        int app_dequeuer();
        void app_task();
        void increment_timer_applic();
        void update_status();

        std::vector<uint8_t> local_msg;
        int local_msg_size = 0;
        bool rcv_msg_reaady = false;
        void rcvd_get_info();

        void cmd_set_mode(Eq3Mode mode);
        void cmd_lock(bool state);
        void cmd_boost(bool state);

        uint8_t eq3_valve_status_;
        void set_eq3_time();
        void send_mode(Eq3Mode mode);
        void send_setpoint_(uint8_t target);
        void send_target_temp(uint8_t temp);
        void send_lock(bool state);
        void send_boost(bool state);
        void rcvd_base_status(ApplicationStatus tipo);

        Eq3StatusResponse  valve_base_status_;

    protected:
  			

	private:
		EQ3NOS *parent_{nullptr};   // puntatore al "padre" (EQ3NOS)	
		QueueHandle_t application_queue_{nullptr};
		ApplicationStatus application_status_;
		
		static constexpr size_t MAX_APP_QUEUE = 20; // numero max comandi in coda
		void get_info();
		void get_current_temp();
		//bool ask_info_= false;	// 
	    //bool rcv_info_ = false; // flag di attesa risposta
	    void parse_msg();
		bool app_task_busy_ = false; 		
		uint32_t timer_appl = 0;
		uint32_t timer_ora = 0;
		uint32_t timer_query = 0;
        uint32_t timer_recovery = 0;
        uint32_t recovery_counter = 0;
		  
};

}  // namespace eq3nos_bt
}  // namespace esphome

