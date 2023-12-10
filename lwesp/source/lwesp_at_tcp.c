// Header: LWESP AT TCP Source File
// File Name: lwesp_at_tcp.c
// Author: Turgay Hopal
// Date: 10.12.2023

#include "lwesp_at_tcp.h"

static volatile lwesp_resp_t lwesp_at_resp_flag;

#if LWESP_CHIP_ESP8266 == 1
static lwesp_at_parameter_t tcp_at_check_connection_p = {
	.cmd_type = LWESP_CMD_TYPE_EXECUTE,
	.cmd_key  = "AT+CIPSTATUS",
	.cmd_params = NULL,
};

#endif

#if LWESP_CHIP_ESP32 == 1
static lwesp_at_parameter_t tcp_at_check_connection_p = {
	.cmd_type = LWESP_CMD_TYPE_QUERY,
	.cmd_key  = "AT+CIPSTATE",
	.cmd_params = NULL,
};

#endif

static lwesp_at_parameter_t tcp_at_resolve_domain_p = {
	.cmd_type = LWESP_CMD_TYPE_SET,
	.cmd_key  = "AT+CIPDOMAIN",
	.cmd_params = NULL,
};

static lwesp_at_parameter_t tcp_at_set_multple_conn_p = {
	.cmd_type = LWESP_CMD_TYPE_SET,
	.cmd_key  = "AT+CIPMUX",
	.cmd_params = NULL,
};

static lwesp_at_parameter_t tcp_at_set_trans_mode_p = {
	.cmd_type = LWESP_CMD_TYPE_SET,
	.cmd_key  = "AT+CIPMODE",
	.cmd_params = NULL,
};

static lwesp_at_parameter_t tcp_at_ping_p = {
	.cmd_type = LWESP_CMD_TYPE_SET,
	.cmd_key  = "AT+PING",
	.cmd_params = NULL,
};


lwesp_resp_t LWESP_TCP_AWAIT_RESP(uint32_t ms) {
	
	TickType_t ticks = xTaskGetTickCount();
  uint32_t start_tick = ticks / (configTICK_RATE_HZ / 1000);
		
	while(lwesp_at_resp_flag == LWESP_RESP_UNKNOW) {
		ticks = xTaskGetTickCount();
		if ((ticks / (configTICK_RATE_HZ / 1000)) - start_tick > ms) {
			lwesp_at_resp_flag = LWESP_RESP_TIMEOUT;
			break;
		}
	}

	return lwesp_at_resp_flag;
}

void lwesp_sys_resp_callback_tcp(lwesp_resp_t resp) {
	switch(resp) {
		case LWESP_RESP_OK:
			printf("******************RESP OK******************\r\n");
			lwesp_at_resp_flag = LWESP_RESP_OK;
			break;
		case LWESP_RESP_ERR:
			printf("******************RESP ERR******************\r\n");
			lwesp_at_resp_flag = LWESP_RESP_ERR;
			break;
		case LWESP_RESP_FAIL:
			break;
		case LWESP_RESP_UNKNOW:
			break;
		case LWESP_RESP_TIMEOUT:
			break;
		case LWESP_RESP_CONF_ERR:
			lwesp_at_resp_flag = LWESP_RESP_CONF_ERR;
			break;
	}
}

void lwesp_at_tcp_focus_on(void) {
	lwesp_sys_set_resp_callback(lwesp_sys_resp_callback_tcp);
}

lwesp_resp_t lwesp_check_conn_status(lwesp_tcp_at_conn_t *conn) {
	
	lwesp_at_resp_flag = LWESP_RESP_UNKNOW;
	lwesp_sys_send_command(tcp_at_check_connection_p);
	
	lwesp_at_resp_flag = LWESP_TCP_AWAIT_RESP(1000);
	
	if (lwesp_at_resp_flag != LWESP_RESP_TIMEOUT) {
		lwesp_sys_at_get_conn_stat(conn);
	}

	return lwesp_at_resp_flag;	
	
}

lwesp_resp_t lwesp_resolve_domain(lwesp_tcp_at_domain_t *domain) {
	
	sprintf((char *)tcp_at_resolve_domain_p.cmd_params, "\"%s\"", domain->domain_name);
	
	lwesp_at_resp_flag = LWESP_RESP_UNKNOW;
	lwesp_sys_send_command(tcp_at_resolve_domain_p);
	
	lwesp_at_resp_flag = LWESP_TCP_AWAIT_RESP(1000);
	
	if (lwesp_at_resp_flag != LWESP_RESP_TIMEOUT) {
		lwesp_sys_at_get_domain_name(domain);
	}

	return lwesp_at_resp_flag;	
	
}

lwesp_resp_t lwesp_set_connection_type(lwesp_at_connection_type_t type) {
	
	sprintf((char *)tcp_at_set_multple_conn_p.cmd_params, "%d", type);
	
	lwesp_at_resp_flag = LWESP_RESP_UNKNOW;
	lwesp_sys_send_command(tcp_at_set_multple_conn_p);
	
	return LWESP_TCP_AWAIT_RESP(1000);
	
}

lwesp_resp_t lwesp_set_transmission_mode(lwesp_at_transmission_mode_t mode) {
	
	sprintf((char *)tcp_at_set_trans_mode_p.cmd_params, "%d", mode);
	
	lwesp_at_resp_flag = LWESP_RESP_UNKNOW;
	lwesp_sys_send_command(tcp_at_set_trans_mode_p);
	
	return LWESP_TCP_AWAIT_RESP(1000);
}

lwesp_resp_t lwesp_ping_ip(lwesp_tcp_at_ping_t *ping) {
	
	sprintf((char *)tcp_at_ping_p.cmd_params, "\"%s\"", ping->ip);
	
	lwesp_at_resp_flag = LWESP_RESP_UNKNOW;
	lwesp_sys_send_command(tcp_at_ping_p);
	
	lwesp_at_resp_flag = LWESP_TCP_AWAIT_RESP(2000);
	
	if (lwesp_at_resp_flag != LWESP_RESP_TIMEOUT) {
		lwesp_sys_at_get_ping_time(ping);
	}

	return lwesp_at_resp_flag;
	
}



