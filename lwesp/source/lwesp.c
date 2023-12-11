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
	client->wifi.lwesp_disconnect_ap = lwesp_disconnect_ap;
	client->wifi.lwesp_create_soft_ap = lwesp_create_soft_ap;
	client->wifi.lwesp_check_soft_ap_devices = lwesp_check_soft_ap_devices;
	client->wifi.lwesp_set_auto_conn_ap = lwesp_set_auto_conn_ap;
	client->wifi.lwesp_set_sta_mac = lwesp_set_sta_mac;
	client->wifi.lwesp_check_sta_mac = lwesp_check_sta_mac;
	client->wifi.lwesp_set_ap_mac = lwesp_set_ap_mac;
	client->wifi.lwesp_check_ap_mac = lwesp_check_ap_mac;
	client->wifi.lwesp_set_sta_ip = lwesp_set_sta_ip;
	client->wifi.lwesp_check_sta_ip = lwesp_check_sta_ip;
	client->wifi.lwesp_set_ap_ip = lwesp_set_ap_ip;
	client->wifi.lwesp_check_ap_ip = lwesp_check_ap_ip;
	
	client->tcp.lwesp_check_conn_status = lwesp_check_conn_status;
	client->tcp.lwesp_resolve_domain = lwesp_resolve_domain;
	client->tcp.lwesp_ping_ip = lwesp_ping_ip;
	client->tcp.lwesp_set_connection_type = lwesp_set_connection_type;
	client->tcp.lwesp_set_transmission_mode = lwesp_set_transmission_mode;
	client->tcp.lwesp_start_connection = lwesp_start_connection;
	
	
	lwesp_sys_init();
	lwesp_sys_set_resp_wifi_callback(wifi_callback);
	
	lwesp_at_basic_focus_on();
	
	lwesp_resp_t resp;

	return resp;
}