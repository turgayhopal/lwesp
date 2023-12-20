
// Header: LWESP System Control Header File
// File Name: lwesp_sys.h
// Author: Turgay Hopal
// Date: 28.11.2023


#ifndef INC_LWESP_SYS_H_
#define INC_LWESP_SYS_H_

#include "lwesp_typedef.h"

#define LWESP_SYS_TX_BUFFER_SIZE		1024
#define LWESP_SYS_RESP_BUFFER_SIZE	1024

#define lwespRX_HANDLER_TASK_PRIORITY				(tskIDLE_PRIORITY + 3UL)
#define lwespRX_HANDLER_TASK_STACK_SIZE			(configMINIMAL_STACK_SIZE)

#ifdef LWESP_MCU_NUVOTON_M480
#include "lwesp_ll_m480.h"
#endif

typedef void (*lwesp_resp_callback)(lwesp_resp_t resp);
typedef void (*lwesp_resp_wifi_callback)(lwesp_resp_wifi_t resp, void *);

typedef struct lwesp_sys_s {
	lwesp_resp_callback resp_callback;
	lwesp_resp_wifi_callback resp_wifi_callback;
}lwesp_sys_t;

void vRxHandlerTask(void *pvParameters);

void lwesp_sys_init(void);
void lwesp_sys_send_command(lwesp_at_parameter_t parameter);

void lwesp_sys_enable_callbacks(void);
void lwesp_sys_disable_callbacks(void);

void lwesp_sys_set_resp_callback(lwesp_resp_callback resp_callback);
void lwesp_sys_set_resp_wifi_callback(lwesp_resp_wifi_callback resp_callback);

void lwesp_sys_at_get_version(lwesp_basic_at_version_t *at_version);
void lwesp_sys_at_get_sleep_mode(lwesp_basic_at_sleep_mode_t *sleep_mode);

#if LWESP_CHIP_ESP8266 == 1
void lwesp_sys_at_get_wifi_mode(lwesp_wifi_at_wifi_mode_t *wifi_mode, uint8_t save_flash_st);
void lwesp_sys_at_get_mac_ap(lwesp_wifi_at_mac_t *mac_addr, uint8_t save_flash_st);
void lwesp_sys_at_get_mac_sta(lwesp_wifi_at_mac_t *mac_addr, uint8_t save_flash_st);
void lwesp_sys_at_get_ip_ap(lwesp_wifi_at_ip_t *ip_addr, uint8_t save_flash_st);
void lwesp_sys_at_get_ip_sta(lwesp_wifi_at_ip_t *ip_addr, uint8_t save_flash_st);
#endif

#if LWESP_CHIP_ESP32 == 1
void lwesp_sys_at_get_wifi_mode(lwesp_wifi_at_wifi_mode_t *wifi_mode);
void lwesp_sys_at_get_mac_ap(lwesp_wifi_at_mac_t *mac_addr);
void lwesp_sys_at_get_mac_sta(lwesp_wifi_at_mac_t *mac_addr);
void lwesp_sys_at_get_ip_ap(lwesp_wifi_at_ip_t *ip_addr);
void lwesp_sys_at_get_ip_sta(lwesp_wifi_at_ip_t *ip_addr);
#endif

void lwesp_sys_at_get_list_ap(void);
void lwesp_sys_at_check_soft_ap_devices(void);
void lwesp_sys_at_get_conn_stat(lwesp_tcp_at_conn_t *conn);
void lwesp_sys_at_get_domain_name(lwesp_tcp_at_domain_t *domain);
void lwesp_sys_at_get_ping_time(lwesp_tcp_at_ping_t *ping);
lwesp_resp_tcp_t lwesp_sys_at_check_tcp_connection(void);
void lwesp_sys_at_get_tcp_response(char *response_body, int *status_code);
void lwesp_sys_at_get_ip_addr(lwesp_tcp_at_get_ip_addr_t *ip);

#endif /* INC_LWESP_SYS_H_ */