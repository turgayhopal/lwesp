// Header: LWESP Source File
// File Name: lwesp.c
// Author: Turgay Hopal
// Date: 29.11.2023

#include "lwesp.h"

lwesp_resp_t lwesp_init(lwesp_client_t *client, lwesp_resp_wifi_callback wifi_callback) {
	
	client->basic.lwesp_check_alive = lwesp_check_alive;
	client->basic.lwesp_check_version = lwesp_check_version;
	client->basic.lwesp_enter_deep_sleep = lwesp_enter_deep_sleep;
	client->basic.lwesp_reset_chip = lwesp_reset_chip;
	client->basic.lwesp_restore_chip = lwesp_restore_chip;
	client->basic.lwesp_set_commands_echo = lwesp_set_commands_echo;
	client->basic.lwesp_set_sleep_mode = lwesp_set_sleep_mode;
	client->basic.lwesp_check_sleep_mode = lwesp_check_sleep_mode;
	client->basic.lwesp_set_rf_power = lwesp_set_rf_power;
	
	client->wifi.lwesp_set_wifi_mode = lwesp_set_wifi_mode;
	client->wifi.lwesp_check_wifi_mode = lwesp_check_wifi_mode;
	client->wifi.lwesp_connect_ap = lwesp_connect_ap;
	client->wifi.lwesp_list_aps = lwesp_list_aps;
	
	lwesp_sys_init();
	lwesp_sys_set_resp_wifi_callback(wifi_callback);
	
	lwesp_at_basic_focus_on();
	
	lwesp_resp_t resp;

	return resp;
}