// Header: LWESP AT Wifi Header File
// File Name: lwesp_at_wifi.h
// Author: Turgay Hopal
// Date: 06.12.2023

#ifndef INC_LWESP_AT_WIFI_H_
#define INC_LWESP_AT_WIFI_H_

#include "lwesp_typedef.h"
#include "lwesp_sys.h"

void lwesp_at_wifi_focus_on(void);

lwesp_resp_t lwesp_set_wifi_mode(lwesp_wifi_at_wifi_mode_t wifi_mode, uint8_t save_flash_st);
lwesp_resp_t lwesp_check_wifi_mode(lwesp_wifi_at_wifi_mode_t *wifi_mode, uint8_t save_flash_st);
lwesp_resp_t lwesp_connect_ap(lwesp_wifi_at_wifi_ap_t wifi_ap, uint8_t save_flash_st);
lwesp_resp_t lwesp_list_aps(void);

#endif /* INC_LWESP_AT_WIFI_H_ */