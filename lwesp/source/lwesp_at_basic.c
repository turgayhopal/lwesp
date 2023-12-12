// Header: LWESP AT Basic Source File
// File Name: lwesp_at_basic.c
// Author: Turgay Hopal
// Date: 29.11.2023

#include "lwesp_at_basic.h"

static volatile lwesp_resp_t lwesp_at_resp_flag;

static lwesp_at_parameter_t basic_at_test_p = {
	.cmd_type = LWESP_CMD_TYPE_EXECUTE,
	.cmd_key  = "AT",
	.cmd_params = NULL
};

static lwesp_at_parameter_t basic_at_reset_p = {
	.cmd_type = LWESP_CMD_TYPE_EXECUTE,
	.cmd_key  = "AT+RST",
	.cmd_params = NULL
};

static lwesp_at_parameter_t basic_at_version_p = {
	.cmd_type = LWESP_CMD_TYPE_EXECUTE,
	.cmd_key  = "AT+GMR",
	.cmd_params = NULL
};
static lwesp_at_parameter_t basic_at_deep_slee_p = {
	.cmd_type = LWESP_CMD_TYPE_SET,
	.cmd_key  = "AT+GSLP",
	.cmd_params = NULL
};

static lwesp_at_parameter_t basic_at_echo_p = {
	.cmd_type = LWESP_CMD_TYPE_EXECUTE,
	.cmd_key  = "ATE",
	.cmd_params = NULL
};

static lwesp_at_parameter_t basic_at_restore_p = {
	.cmd_type = LWESP_CMD_TYPE_EXECUTE,
	.cmd_key  = "AT+RESTORE",
	.cmd_params = NULL
};

static lwesp_at_parameter_t basic_at_check_sleep_mode_p = {
	.cmd_type = LWESP_CMD_TYPE_QUERY,
	.cmd_key  = "AT+SLEEP",
	.cmd_params = NULL
};

static lwesp_at_parameter_t basic_at_set_sleep_mode_p = {
	.cmd_type = LWESP_CMD_TYPE_SET,
	.cmd_key  = "AT+SLEEP",
	.cmd_params = NULL
};

static lwesp_at_parameter_t basic_at_set_rf_power_p = {
	.cmd_type = LWESP_CMD_TYPE_SET,
	.cmd_key  = "AT+RFPOWER",
	.cmd_params = NULL
};

lwesp_resp_t LWESP_BASIC_AWAIT_RESP(uint32_t ms) {
	
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

void lwesp_sys_resp_callback_basic(lwesp_resp_t resp) {
	switch(resp) {
		case LWESP_RESP_OK:
			printf("******************RESP OK******************\r\n");
			lwesp_at_resp_flag = LWESP_RESP_OK;
			break;
		case LWESP_RESP_ERR:
			printf("******************RESP ERR******************\r\n");
			lwesp_at_resp_flag = LWESP_RESP_ERR;
			break;
		case LWESP_RESP_UNKNOW:
			break;
		case LWESP_RESP_FAIL:
			break;
		case LWESP_RESP_TIMEOUT:
			break;
		case LWESP_RESP_RECV_IPD:
			break;
		case LWESP_RESP_CONF_ERR:
			lwesp_at_resp_flag = LWESP_RESP_CONF_ERR;
			break;
	}
}

void lwesp_at_basic_focus_on(void) {
	lwesp_sys_set_resp_callback(lwesp_sys_resp_callback_basic);
}

lwesp_resp_t lwesp_check_alive(void) {
	
	lwesp_at_resp_flag = LWESP_RESP_UNKNOW;
	lwesp_sys_send_command(basic_at_test_p);
	
	return LWESP_BASIC_AWAIT_RESP(100);
}

lwesp_resp_t lwesp_reset_chip(void) {
	lwesp_at_resp_flag = LWESP_RESP_UNKNOW;
	lwesp_sys_send_command(basic_at_reset_p);
	
	return LWESP_BASIC_AWAIT_RESP(2000);
}

lwesp_resp_t lwesp_check_version(lwesp_basic_at_version_t *at_version) {
	
	lwesp_at_resp_flag = LWESP_RESP_UNKNOW;
	
	lwesp_sys_send_command(basic_at_version_p);
	
	lwesp_at_resp_flag = LWESP_BASIC_AWAIT_RESP(100);
	
	if (lwesp_at_resp_flag != LWESP_RESP_TIMEOUT) {
		lwesp_sys_at_get_version(at_version);
	}

	return lwesp_at_resp_flag;
}

lwesp_resp_t lwesp_enter_deep_sleep(uint32_t time_ms) {

	sprintf((char *)basic_at_deep_slee_p.cmd_params, "%d", time_ms);
	
	lwesp_at_resp_flag = LWESP_RESP_UNKNOW;
	lwesp_sys_send_command(basic_at_deep_slee_p);
	
	return LWESP_BASIC_AWAIT_RESP(100);
}

lwesp_resp_t lwesp_set_commands_echo(lwesp_at_echo_t echo) {
	
	sprintf((char *)basic_at_echo_p.cmd_key, "%s%d", basic_at_echo_p.cmd_key, echo);
		
	lwesp_at_resp_flag = LWESP_RESP_UNKNOW;
	lwesp_sys_send_command(basic_at_echo_p);
	
	return LWESP_BASIC_AWAIT_RESP(100);
}

lwesp_resp_t lwesp_restore_chip(void) {
	lwesp_at_resp_flag = LWESP_RESP_UNKNOW;
	lwesp_sys_send_command(basic_at_restore_p);
	
	return LWESP_BASIC_AWAIT_RESP(2000);
}

lwesp_resp_t lwesp_set_sleep_mode(lwesp_basic_at_sleep_mode_t sleep_mode) {
	
	sprintf((char *)basic_at_set_sleep_mode_p.cmd_params, "%s", sleep_mode.sleep_mode);
	
	lwesp_at_resp_flag = LWESP_RESP_UNKNOW;
	lwesp_sys_send_command(basic_at_set_sleep_mode_p);
	
	return LWESP_BASIC_AWAIT_RESP(100);
}

lwesp_resp_t lwesp_check_sleep_mode(lwesp_basic_at_sleep_mode_t *sleep_mode) {
	
	lwesp_at_resp_flag = LWESP_RESP_UNKNOW;
	lwesp_sys_send_command(basic_at_check_sleep_mode_p);
	
	lwesp_at_resp_flag = LWESP_BASIC_AWAIT_RESP(100);
	
	if (lwesp_at_resp_flag != LWESP_RESP_TIMEOUT) {
		lwesp_sys_at_get_sleep_mode(sleep_mode);
	}
	
	return lwesp_at_resp_flag;
}

lwesp_resp_t lwesp_set_rf_power(lwesp_basic_at_rf_power_t rf_power) {
	sprintf((char *)basic_at_set_rf_power_p.cmd_params, "%s", rf_power.rf_power);
	
	lwesp_at_resp_flag = LWESP_RESP_UNKNOW;
	lwesp_sys_send_command(basic_at_set_rf_power_p);
	
	return LWESP_BASIC_AWAIT_RESP(100);
}
