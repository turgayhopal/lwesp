// Header: LWESP API Header File
// File Name: lwesp_api.h
// Author: Turgay Hopal
// Date: 15.12.2023

#ifndef INC_LWESP_API_H_
#define INC_LWESP_API_H_

#include "lwesp.h"

lwesp_api_resp_t lwesp_api_init(lwesp_resp_wifi_callback wifi_callback);
lwesp_api_resp_t lwesp_api_connect_ap(char *ssid, char *password, uint8_t auto_conn_st);

#endif /* INC_LWESP_API_H_ */