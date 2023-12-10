// Header: LWESP AT Wifi Header File
// File Name: lwesp_at_wifi.h
// Author: Turgay Hopal
// Date: 06.12.2023

#ifndef INC_LWESP_AT_WIFI_H_
#define INC_LWESP_AT_WIFI_H_

#include "lwesp_typedef.h"
#include "lwesp_sys.h"

void lwesp_at_wifi_focus_on(void);

#if LWESP_CHIP_ESP8266 == 1
lwesp_resp_t lwesp_set_wifi_mode(lwesp_wifi_at_wifi_mode_t wifi_mode, uint8_t save_flash_st);
lwesp_resp_t lwesp_check_wifi_mode(lwesp_wifi_at_wifi_mode_t *wifi_mode, uint8_t save_flash_st);
lwesp_resp_t lwesp_connect_ap(lwesp_wifi_at_wifi_ap_t wifi_ap, uint8_t save_flash_st);
lwesp_resp_t lwesp_create_soft_ap(lwesp_wifi_at_create_soft_ap_t soft_ap, uint8_t save_flash_st);
lwesp_resp_t lwesp_check_sta_mac(lwesp_wifi_at_mac_t *mac_addr, uint8_t save_flash_st);
lwesp_resp_t lwesp_set_sta_mac(lwesp_wifi_at_mac_t mac_addr, uint8_t save_flash_st);
lwesp_resp_t lwesp_check_ap_mac(lwesp_wifi_at_mac_t *mac_addr, uint8_t save_flash_st);
lwesp_resp_t lwesp_set_ap_mac(lwesp_wifi_at_mac_t mac_addr, uint8_t save_flash_st);
#endif 

#if LWESP_CHIP_ESP32 == 1
lwesp_resp_t lwesp_set_wifi_mode(lwesp_wifi_at_wifi_mode_t wifi_mode);
lwesp_resp_t lwesp_check_wifi_mode(lwesp_wifi_at_wifi_mode_t *wifi_mode);
lwesp_resp_t lwesp_connect_ap(lwesp_wifi_at_wifi_ap_t wifi_ap);
lwesp_resp_t lwesp_create_soft_ap(lwesp_wifi_at_create_soft_ap_t soft_ap);
lwesp_resp_t lwesp_check_sta_mac(lwesp_wifi_at_mac_t *mac_addr);
lwesp_resp_t lwesp_set_sta_mac(lwesp_wifi_at_mac_t mac_addr);
lwesp_resp_t lwesp_check_ap_mac(lwesp_wifi_at_mac_t *mac_addr);
lwesp_resp_t lwesp_set_ap_mac(lwesp_wifi_at_mac_t mac_addr);
#endif

lwesp_resp_t lwesp_list_aps(void);
lwesp_resp_t lwesp_disconnect_ap(void);
lwesp_resp_t lwesp_check_soft_ap_devices(void);
lwesp_resp_t lwesp_set_auto_conn_ap(uint8_t status);

#endif /* INC_LWESP_AT_WIFI_H_ */