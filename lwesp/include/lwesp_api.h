// Header: LWESP API Header File
// File Name: lwesp_api.h
// Author: Turgay Hopal
// Date: 15.12.2023

#ifndef INC_LWESP_API_H_
#define INC_LWESP_API_H_

#include "lwesp.h"

lwesp_api_resp_t lwesp_api_init(lwesp_resp_wifi_callback wifi_callback);
lwesp_api_resp_t lwesp_api_connect_ap(char *ssid, char *password, uint8_t auto_conn_st);
lwesp_api_resp_t lwesp_api_get_ip_mac_addr(lwesp_tcp_at_get_ip_addr_t *ip_mac_addr);
lwesp_api_resp_t lwesp_api_http_get(char *host, char *port, char* url, char *response_body, int *status_code, uint16_t timeout);
lwesp_api_resp_t lwesp_api_ping(lwesp_tcp_at_ping_t *ping);

#endif /* INC_LWESP_API_H_ */