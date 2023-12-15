
// Header: LWESP API Source File
// File Name: lwesp_api.c
// Author: Turgay Hopal
// Date: 15.12.2023

#include "lwesp_api.h"

static lwesp_client_t lwesp_client =  {0};

lwesp_api_resp_t lwesp_api_init(lwesp_resp_wifi_callback wifi_callback) {
	
	lwesp_init(&lwesp_client, wifi_callback);
	
	if (lwesp_client.basic.lwesp_check_alive() == LWESP_RESP_OK) {
		
		lwesp_client.basic.lwesp_set_commands_echo(LWESP_AT_ECHO_OFF);
		
		return LWESP_API_CONF_OK;
	}
	
	return LWESP_API_CONF_ERR;
	
}

lwesp_api_resp_t lwesp_api_connect_ap(char *ssid, char *password, uint8_t auto_conn_st) {
	
	lwesp_at_wifi_focus_on();
	
	lwesp_wifi_at_wifi_mode_t wifi_mode;	
	sprintf((char *)wifi_mode.wifi_mode, "3");

	if (lwesp_client.wifi.lwesp_set_wifi_mode(wifi_mode) != LWESP_RESP_OK) {
		return LWESP_API_CONF_ERR;
	}
	
	if (lwesp_client.wifi.lwesp_set_auto_conn_ap(auto_conn_st) != LWESP_RESP_OK) {
		return LWESP_API_CONF_ERR;
	}
	
	lwesp_wifi_at_wifi_ap_t ap;
	
	sprintf((char *)ap.ssid, "%s", ssid);
	sprintf((char *)ap.passwd, "%s", password);

	if (lwesp_client.wifi.lwesp_connect_ap(ap) != LWESP_RESP_OK) {
		return LWESP_API_CONF_ERR;
	}

	return LWESP_API_CONF_OK;
}
