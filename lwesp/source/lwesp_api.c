
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

lwesp_api_resp_t lwesp_api_get_ip_mac_addr(lwesp_tcp_at_get_ip_addr_t *ip) {
	
	lwesp_at_tcp_focus_on();
			
	if (lwesp_client.tcp.lwesp_get_ip_addr(ip) == LWESP_RESP_OK) {
		printf("Wifi Driver AP IP   : ............. [%s]\r\n", ip->ap_ip_addr);
		printf("Wifi Driver AP MAC  : ............. [%s]\r\n", ip->ap_mac_addr);
		printf("Wifi Driver STA IP  : ............. [%s]\r\n", ip->sta_ip_addr);
		printf("Wifi Driver STA MAC : ............. [%s]\r\n", ip->sta_mac_addr);
		return LWESP_API_CONF_OK;
	}
	return LWESP_API_CONF_ERR;
}

lwesp_api_resp_t lwesp_api_http_get(char *host, char *port, char* url, char *response_body, int *status_code, uint16_t timeout) {

	lwesp_resp_t resp = LWESP_RESP_UNKNOW;
	
	lwesp_at_tcp_focus_on();
	
	lwesp_tcp_at_start_tcp_conn_t tcp_conn;
	
	sprintf((char *) tcp_conn.type, "TCP");
	sprintf((char *) tcp_conn.keep_alive, "%d", timeout);
	sprintf((char *) tcp_conn.remote_host, "%s", host);
	sprintf((char *) tcp_conn.remote_port, "%s", port);
	
	resp = lwesp_client.tcp.lwesp_start_tcp_connection(tcp_conn, LWESP_AT_CONN_TYPE_SINGLE);
	
	if (resp == LWESP_RESP_OK) {
		
		lwesp_tcp_at_send_data_t data;
		
		sprintf((char *)data.data, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", url, host);
		
		resp = LWESP_RESP_UNKNOW;
		resp = lwesp_client.tcp.lwesp_send_data_lenght(data, 0, LWESP_AT_CONN_TYPE_SINGLE);
		
		if (resp == LWESP_RESP_OK) {
			
			resp = LWESP_RESP_UNKNOW;
			resp = lwesp_client.tcp.lwesp_send_data(data, response_body, status_code);
			
			if (resp == LWESP_RESP_OK) {
				
				resp = LWESP_RESP_UNKNOW;
				resp = lwesp_client.tcp.lwesp_close_connection(0, LWESP_AT_CONN_TYPE_SINGLE);

				if (resp == LWESP_RESP_OK) {
					return LWESP_API_CONF_OK;
				}				
			}
		}
	}
	

	return LWESP_API_CONF_ERR;
}

lwesp_api_resp_t lwesp_api_ping(lwesp_tcp_at_ping_t *ping) {
	
	lwesp_at_tcp_focus_on();
	
	if (lwesp_client.tcp.lwesp_ping_ip(ping) == LWESP_RESP_OK) {
		return LWESP_API_CONF_OK;
	}
	
	return LWESP_API_CONF_ERR;
}