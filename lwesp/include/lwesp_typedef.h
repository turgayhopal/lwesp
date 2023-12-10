// Header: LWESP Typedef Header File
// File Name: lwesp.h
// Author: Turgay Hopal
// Date: 28.11.2023


#ifndef INC_LWESP_TYPEDEF_H_
#define INC_LWESP_TYPEDEF_H_

#include <stdint.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

#include "lwesp_queue.h"

#include "lwesp_config.h"

typedef enum lwesp_resp_e{
	LWESP_RESP_OK,
	LWESP_RESP_ERR,
	LWESP_RESP_FAIL,
	LWESP_RESP_UNKNOW,
	LWESP_RESP_TIMEOUT,
	LWESP_RESP_CONF_ERR
} lwesp_resp_t;	

typedef enum lwesp_resp_basic_e {
	LWESP_RESP_BASIC_READY
} lwesp_resp_basic_t;

typedef enum lwesp_resp_wifi_e {
	LWESP_RESP_WIFI_DISCONNECT,
	LWESP_RESP_WIFI_CONNECTED,
	LWESP_RESP_WIFI_GOT_IP,
	LWESP_RESP_WIFI_AP_FOUND,
	LWESP_RESP_WIFI_STA_CONNECTED,
	LWESP_RESP_WIFI_STA_DISCONNECTED,
	LWESP_RESP_WIFI_STA_GET_IP,
} lwesp_resp_wifi_t;

typedef enum lwesp_cmd_type_e{
	LWESP_CMD_TYPE_TEST,
	LWESP_CMD_TYPE_QUERY,
	LWESP_CMD_TYPE_SET,
	LWESP_CMD_TYPE_EXECUTE
} lwesp_cmd_type_t;

typedef enum lwesp_at_echo_e{
	LWESP_AT_ECHO_OFF = 0x00,
	LWESP_AT_ECHO_ON = 0x01
}lwesp_at_echo_t;

typedef struct lwesp_at_parameter_s {
	lwesp_cmd_type_t cmd_type;
	uint8_t cmd_key[30];
	uint8_t cmd_params[100];
} lwesp_at_parameter_t;

typedef struct lwesp_basic_at_version_s {
	uint8_t at_version_info[30];
	uint8_t sdk_version_info[30];
	uint8_t bin_version[30];
} lwesp_basic_at_version_t;

typedef struct lwesp_basic_at_sleep_mode_s {
	uint8_t sleep_mode[2];
} lwesp_basic_at_sleep_mode_t;

typedef struct lwesp_basic_at_rf_power_s {
	uint8_t rf_power[2];
} lwesp_basic_at_rf_power_t;

typedef struct lwesp_basic_command_s {
	lwesp_resp_t (*lwesp_check_alive)				(void);
	lwesp_resp_t (*lwesp_reset_chip)				(void);
	lwesp_resp_t (*lwesp_check_version)			(lwesp_basic_at_version_t *at_version_t);
	lwesp_resp_t (*lwesp_enter_deep_sleep)	(uint32_t time_ms);
	lwesp_resp_t (*lwesp_set_commands_echo)	(lwesp_at_echo_t echo);
	lwesp_resp_t (*lwesp_restore_chip)			(void);
	lwesp_resp_t (*lwesp_check_sleep_mode)	(lwesp_basic_at_sleep_mode_t *sleep_mode);
	lwesp_resp_t (*lwesp_set_sleep_mode)		(lwesp_basic_at_sleep_mode_t sleep_mode);
	lwesp_resp_t (*lwesp_set_rf_power)			(lwesp_basic_at_rf_power_t rf_power);
} lwesp_basic_command_t;

typedef struct lwesp_wifi_at_wifi_mode_s {
	uint8_t wifi_mode[2];
}lwesp_wifi_at_wifi_mode_t;

typedef struct lwesp_wifi_at_wifi_ap_s {
	uint8_t ssid[50];
	uint8_t passwd[50];
}lwesp_wifi_at_wifi_ap_t;

typedef struct lwesp_wifi_at_create_soft_ap_s {
	uint8_t ssid[50];
	uint8_t passwd[50];
	uint8_t channel;
	uint8_t ecn;
}lwesp_wifi_at_create_soft_ap_t;

typedef struct lwesp_wifi_at_mac_s {
	uint8_t mac_addr[20];
}lwesp_wifi_at_mac_t;

typedef struct lwesp_wifi_at_ip_s {
	uint8_t ip[20];
	uint8_t gateway[20];
	uint8_t netmask[20];
}lwesp_wifi_at_ip_t;


typedef struct lwesp_wifi_command_s {
#if LWESP_CHIP_ESP8266 == 1
	lwesp_resp_t (*lwesp_set_wifi_mode)	 	(lwesp_wifi_at_wifi_mode_t wifi_mode, uint8_t save_flash_st);
	lwesp_resp_t (*lwesp_check_wifi_mode)	(lwesp_wifi_at_wifi_mode_t *wifi_mode, uint8_t save_flash_st);
	lwesp_resp_t (*lwesp_connect_ap)		 	(lwesp_wifi_at_wifi_ap_t wifi_ap, uint8_t save_flash_st);
	lwesp_resp_t (*lwesp_create_soft_ap)  (lwesp_wifi_at_create_soft_ap_t soft_ap, uint8_t save_flash_st);
	lwesp_resp_t (*lwesp_check_sta_mac)   (lwesp_wifi_at_mac_t *mac_addr, uint8_t save_flash_st);
	lwesp_resp_t (*lwesp_set_sta_mac)   	(lwesp_wifi_at_mac_t mac_addr, uint8_t save_flash_st);
	lwesp_resp_t (*lwesp_check_ap_mac)   	(lwesp_wifi_at_mac_t *mac_addr, uint8_t save_flash_st);
	lwesp_resp_t (*lwesp_set_ap_mac)   		(lwesp_wifi_at_mac_t mac_addr, uint8_t save_flash_st);
	lwesp_resp_t (*lwesp_set_ap_ip)       (lwesp_wifi_at_ip_t ip_addr, uint8_t save_flash_st);
	lwesp_resp_t (*lwesp_check_ap_ip)     (lwesp_wifi_at_ip_t *ip_addr, uint8_t save_flash_st);
	lwesp_resp_t (*lwesp_set_sta_ip)      (lwesp_wifi_at_ip_t ip_addr, uint8_t save_flash_st);
	lwesp_resp_t (*lwesp_check_sta_ip)    (lwesp_wifi_at_ip_t *ip_addr, uint8_t save_flash_st);
#endif
#if LWESP_CHIP_ESP32 == 1
	lwesp_resp_t (*lwesp_set_wifi_mode)	 	(lwesp_wifi_at_wifi_mode_t wifi_mode);
	lwesp_resp_t (*lwesp_check_wifi_mode)	(lwesp_wifi_at_wifi_mode_t *wifi_mode);
	lwesp_resp_t (*lwesp_connect_ap)		 	(lwesp_wifi_at_wifi_ap_t wifi_ap);
	lwesp_resp_t (*lwesp_create_soft_ap)  (lwesp_wifi_at_create_soft_ap_t soft_ap);
	lwesp_resp_t (*lwesp_check_sta_mac)   (lwesp_wifi_at_mac_t *mac_addr);
	lwesp_resp_t (*lwesp_set_sta_mac)   	(lwesp_wifi_at_mac_t mac_addr);
	lwesp_resp_t (*lwesp_check_ap_mac)   	(lwesp_wifi_at_mac_t *mac_addr);
	lwesp_resp_t (*lwesp_set_ap_mac)   		(lwesp_wifi_at_mac_t mac_addr);
	lwesp_resp_t (*lwesp_set_ap_ip)       (lwesp_wifi_at_ip_t ip_addr);
	lwesp_resp_t (*lwesp_check_ap_ip)     (lwesp_wifi_at_ip_t *ip_addr);
	lwesp_resp_t (*lwesp_set_sta_ip)      (lwesp_wifi_at_ip_t ip_addr);
	lwesp_resp_t (*lwesp_check_sta_ip)    (lwesp_wifi_at_ip_t *ip_addr);
	
#endif
	lwesp_resp_t (*lwesp_list_aps) 							(void);
	lwesp_resp_t (*lwesp_disconnect_ap)   			(void);
	lwesp_resp_t (*lwesp_check_soft_ap_devices) (void);
	lwesp_resp_t (*lwesp_set_auto_conn_ap)      (uint8_t status);
} lwesp_wifi_command_t;

typedef struct lwesp_client_s {
	lwesp_basic_command_t basic;
	lwesp_wifi_command_t  wifi;
} lwesp_client_t;

typedef struct lwesp_ll_s {
	lwesp_resp_t (*lwesp_ll_configure_uart_clock)(void);
	lwesp_resp_t (*lwesp_ll_configure_pin)(void);
	lwesp_resp_t (*lwesp_ll_configure_uart)(void);
	lwesp_resp_t (*lwesp_ll_configure_uart_irq)(void);
	lwesp_resp_t (*lwesp_ll_hard_reset)(uint8_t state);
	lwesp_resp_t (*lwesp_ll_send_data)(uint8_t *data, uint32_t size);
} lwesp_ll_t;

#endif /* INC_LWESP_TYPEDEF_H_ */