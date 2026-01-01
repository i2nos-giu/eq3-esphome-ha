#pragma once

#include "esphome/core/component.h"
#include "esphome/components/ble_client/ble_client.h"
#include "esphome/core/log.h"
#include <vector>
#include "eq3nos_conncommand.h"

#define DISCONNECT_NOW 3 // disconnecti dopo 3 sec

namespace esphome {
namespace eq3nos_bt {

class EQ3NOS;  // forward declaration

enum ConnectionStatus {
    WAIT_STATE,
    START_SESSION,
    CONNECTION_REQUESTED,
    CONNECTED_SUCCEFULLY, 
    WAIT_RESOLV,
    SESSION_END,
    AFTER_CONNECTED,
    SEND_COMMAND,
    WAIT_REPLAY,
    SEND_REPLAY_TO_APP,
    WAIT_TO_DISCONNECT,
    CONNECT_ERROR,
    GENERIC_DISCONNECT,
    DISCONNECT,
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
};

class EQ3Connection {
public:
    //EQ3Connection();
    void set_parent(EQ3NOS *parent);
    
    void increment_timer_connect(); // Timer per gestione task
    void connect_task();
	void con_task_init();
	bool connect_queuer(const ConnCommand &cmd);
	int conn_dequeuer();
	void session_Processor();
	void session_RAW(const uint8_t *data, size_t len);

    // callback ble
    void on_ble_event(esp_gattc_cb_event_t event,
	    esp_gatt_if_t gatt_if,
    esp_ble_gattc_cb_param_t *param);
        
    esphome::esp32_ble_client::BLECharacteristic *write_ch_ = nullptr;
		esphome::esp32_ble_client::BLECharacteristic *notify_ch_ = nullptr;
     
    uint16_t service_handle_{0};
    uint16_t write_handle_{0};
    uint16_t notify_handle_{0};
    uint16_t cccd_handle_{0}; 
         
         
    // Connessione BLE
    //void start_connection();
   // void close_connection();
    void on_services_resolved_manual();
    void after_connected_();
    void send_replay_to_app();
    
		// Abilita notifiche per una caratteristica
		//void enable_notifications(uint16_t char_handle);

    // Invio comandi
    //void send_command(const uint8_t *data, size_t len);
    void send_command();


    void wait_to_reconnect_set();
    std::vector<uint8_t> last_notify_data_;
	std::vector<uint8_t> local_copy;

private:
    EQ3NOS *parent_{nullptr};
    QueueHandle_t connection_queue_{nullptr};
    ConnectionStatus connection_status_;
    //const uint8_t *temp_cmd_data[16];
    uint8_t temp_cmd_data[16];
    size_t temp_cmd_len = 0;
	uint32_t timer_connect = 0;
    bool resolved = false;
    bool rcv_replay = false;
    bool conn_task_busy_ = false;
    uint32_t disconnect_delay = 0;
    void set_task_busy();
    void reset_task_busy();
    // Buffer per comando in invio
    std::vector<uint8_t> command_buffer_;
    static constexpr size_t MAX_CONN_QUEUE = 20; // numero max comandi in coda
};

}  // namespace eq3nos
}  // namespace esphome

