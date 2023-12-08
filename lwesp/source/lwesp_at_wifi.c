// Header: LWESP AT Wifi Source File
// File Name: lwesp_at_wifi.c
// Author: Turgay Hopal
// Date: 06.12.2023

#include "lwesp_at_wifi.h"

static volatile lwesp_resp_t lwesp_at_resp_flag;


static lwesp_at_parameter_t wifi_at_check_wifi_mode_p = {
	.cmd_type = LWESP_CMD_TYPE_QUERY,
	.cmd_key  = "AT+CWMODE",
	.cmd_params = NULL,
};

static lwesp_at_parameter_t wifi_at_set_wifi_mode_p = {
	.cmd_type = LWESP_CMD_TYPE_SET,
	.cmd_key  = "AT+CWMODE",
	.cmd_params = NULL,
};

static lwesp_at_parameter_t wifi_at_check_ap_p = {
	.cmd_type = LWESP_CMD_TYPE_QUERY,
	.cmd_key  = "AT+CWJAP",
	.cmd_params = NULL,
};

static lwesp_at_parameter_t wifi_at_connect_ap_p = {
	.cmd_type = LWESP_CMD_TYPE_SET,
	.cmd_key  = "AT+CWJAP",
	.cmd_params = NULL
};

static lwesp_at_parameter_t wifi_at_list_ap_p = {
	.cmd_type = LWESP_CMD_TYPE_EXECUTE,
	.cmd_key  = "AT+CWLAP",
	.cmd_params = NULL
};

static lwesp_at_parameter_t wifi_at_disconnect_ap_p = {
	.cmd_type = LWESP_CMD_TYPE_EXECUTE,
	.cmd_key  = "AT+CWQAP",
	.cmd_params = NULL
};

static lwesp_at_parameter_t wifi_at_create_soft_ap_p = {
	.cmd_type = LWESP_CMD_TYPE_SET,
	.cmd_key  = "AT+CWSAP",
	.cmd_params = NULL
};

lwesp_resp_t LWESP_WIFI_AWAIT_RESP(uint32_t ms) {
	
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

void lwesp_sys_resp_callback_wifi(lwesp_resp_t resp) {
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

void lwesp_at_wifi_focus_on(void) {
	lwesp_sys_set_resp_callback(lwesp_sys_resp_callback_wifi);
}

#if LWESP_CHIP_ESP8266 == 1

lwesp_resp_t lwesp_set_wifi_mode(lwesp_wifi_at_wifi_mode_t wifi_mode, uint8_t save_flash_st) {
	
	if (save_flash_st) {
		sprintf((char *)wifi_at_set_wifi_mode_p.cmd_key, "%s_DEF", wifi_at_set_wifi_mode_p.cmd_key);
	} else {
		sprintf((char *)wifi_at_set_wifi_mode_p.cmd_key, "%s_CUR", wifi_at_set_wifi_mode_p.cmd_key);
	}
	
	sprintf((char *)wifi_at_set_wifi_mode_p.cmd_params, "%s", wifi_mode.wifi_mode);
	
	lwesp_at_resp_flag = LWESP_RESP_UNKNOW;
	lwesp_sys_send_command(wifi_at_set_wifi_mode_p);
	
	return LWESP_WIFI_AWAIT_RESP(100);
}

lwesp_resp_t lwesp_check_wifi_mode(lwesp_wifi_at_wifi_mode_t *wifi_mode, uint8_t save_flash_st) {
	
	if (save_flash_st) {
		sprintf((char *)wifi_at_check_wifi_mode_p.cmd_key, "%s_DEF", wifi_at_check_wifi_mode_p.cmd_key);
	} else {
		sprintf((char *)wifi_at_check_wifi_mode_p.cmd_key, "%s_CUR", wifi_at_check_wifi_mode_p.cmd_key);
	}
	
	lwesp_at_resp_flag = LWESP_RESP_UNKNOW;
	lwesp_sys_send_command(wifi_at_check_wifi_mode_p);
	
	lwesp_at_resp_flag = LWESP_WIFI_AWAIT_RESP(100);
	
	if (lwesp_at_resp_flag != LWESP_RESP_TIMEOUT) {
		lwesp_sys_at_get_wifi_mode(wifi_mode, save_flash_st);
	}

	return lwesp_at_resp_flag;
}

lwesp_resp_t lwesp_connect_ap(lwesp_wifi_at_wifi_ap_t wifi_ap, uint8_t save_flash_st) {
	
	if (save_flash_st) {
		sprintf((char *)wifi_at_connect_ap_p.cmd_key, "%s_DEF", wifi_at_connect_ap_p.cmd_key);
	} else {
		sprintf((char *)wifi_at_connect_ap_p.cmd_key, "%s_CUR", wifi_at_connect_ap_p.cmd_key);
	}
	
	sprintf((char *)wifi_at_connect_ap_p.cmd_params, "\"%s\",\"%s\"", wifi_ap.ssid, wifi_ap.passwd);
	
	lwesp_at_resp_flag = LWESP_RESP_UNKNOW;
	lwesp_sys_send_command(wifi_at_connect_ap_p);
	
	return LWESP_WIFI_AWAIT_RESP(15000);
	
}

lwesp_resp_t lwesp_create_soft_ap(lwesp_wifi_at_create_soft_ap_t soft_ap, uint8_t save_flash_st) {

	if (save_flash_st) {
		sprintf((char *)wifi_at_create_soft_ap_p.cmd_key, "%s_DEF", wifi_at_create_soft_ap_p.cmd_key);
	} else {
		sprintf((char *)wifi_at_create_soft_ap_p.cmd_key, "%s_CUR", wifi_at_create_soft_ap_p.cmd_key);
	}
	
	sprintf((char *)wifi_at_create_soft_ap_p.cmd_params, "\"%s\",\"%s\",%d,%d", soft_ap.ssid, soft_ap.passwd, soft_ap.channel, soft_ap.securty_enc);
	
	lwesp_at_resp_flag = LWESP_RESP_UNKNOW;
	lwesp_sys_send_command(wifi_at_create_soft_ap_p);
	
	return LWESP_WIFI_AWAIT_RESP(1000);
}

#endif

#if LWESP_CHIP_ESP32 == 1

lwesp_resp_t lwesp_set_wifi_mode(lwesp_wifi_at_wifi_mode_t wifi_mode) {
	
	sprintf((char *)wifi_at_set_wifi_mode_p.cmd_params, "%s", wifi_mode.wifi_mode);
	
	lwesp_at_resp_flag = LWESP_RESP_UNKNOW;
	lwesp_sys_send_command(wifi_at_set_wifi_mode_p);
	
	return LWESP_WIFI_AWAIT_RESP(100);
	
}

lwesp_resp_t lwesp_check_wifi_mode(lwesp_wifi_at_wifi_mode_t *wifi_mode) {

	lwesp_at_resp_flag = LWESP_RESP_UNKNOW;
	lwesp_sys_send_command(wifi_at_check_wifi_mode_p);
	
	lwesp_at_resp_flag = LWESP_WIFI_AWAIT_RESP(100);
	
	if (lwesp_at_resp_flag != LWESP_RESP_TIMEOUT) {
		lwesp_sys_at_get_wifi_mode(wifi_mode);
	}

	return lwesp_at_resp_flag;
}

lwesp_resp_t lwesp_connect_ap(lwesp_wifi_at_wifi_ap_t wifi_ap) {

	sprintf((char *)wifi_at_connect_ap_p.cmd_params, "\"%s\",\"%s\"", wifi_ap.ssid, wifi_ap.passwd);
	
	lwesp_at_resp_flag = LWESP_RESP_UNKNOW;
	lwesp_sys_send_command(wifi_at_connect_ap_p);
	
	return LWESP_WIFI_AWAIT_RESP(15000);;
}

lwesp_resp_t lwesp_create_soft_ap(lwesp_wifi_at_create_soft_ap_t soft_ap) {
	
	sprintf((char *)wifi_at_create_soft_ap_p.cmd_params, "\"%s\",\"%s\",%d,%d", soft_ap.ssid, soft_ap.passwd, soft_ap.channel, soft_ap.ecn);
	
	lwesp_at_resp_flag = LWESP_RESP_UNKNOW;
	lwesp_sys_send_command(wifi_at_create_soft_ap_p);
	
	return LWESP_WIFI_AWAIT_RESP(1000);
	
}

#endif

lwesp_resp_t lwesp_list_aps(void) {
		
	lwesp_at_resp_flag = LWESP_RESP_UNKNOW;
	lwesp_sys_send_command(wifi_at_list_ap_p);
	
	lwesp_at_resp_flag = LWESP_WIFI_AWAIT_RESP(4000);
	
	if (lwesp_at_resp_flag != LWESP_RESP_TIMEOUT) {
		lwesp_sys_at_get_list_ap();
	}

	return lwesp_at_resp_flag;
	
}

lwesp_resp_t lwesp_disconnect_ap(void) {
		
	lwesp_at_resp_flag = LWESP_RESP_UNKNOW;
	lwesp_sys_send_command(wifi_at_disconnect_ap_p);
	
	return LWESP_WIFI_AWAIT_RESP(1000);;
	
}