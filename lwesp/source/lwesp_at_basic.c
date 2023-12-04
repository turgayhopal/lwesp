// Header: LWESP AT Basic Source File
// File Name: lwesp_at_basic.c
// Author: Turgay Hopal
// Date: 29.11.2023

#include "lwesp_at_basic.h"

static volatile lwesp_resp_t lwesp_at_resp_flag;

static lwesp_at_parameter_t basic_at_test_p = {
	.cmd_type = LWESP_CMD_TYPE_EXECUTE,
	.cmd_key  = "AT"
};

static lwesp_at_parameter_t basic_at_reset_p = {
	.cmd_type = LWESP_CMD_TYPE_EXECUTE,
	.cmd_key  = "AT+RST"
};

static lwesp_at_parameter_t basic_at_version_p = {
	.cmd_type = LWESP_CMD_TYPE_EXECUTE,
	.cmd_key  = "AT+GMR"

};
static lwesp_at_parameter_t basic_at_deep_slee_p = {
	.cmd_type = LWESP_CMD_TYPE_SET,
	.cmd_key  = "AT+GSLP"
};

static lwesp_at_parameter_t basic_at_echo_p = {
	.cmd_type = LWESP_CMD_TYPE_EXECUTE,
	.cmd_key  = "ATE",
};

static lwesp_at_parameter_t basic_at_restore_p = {
	.cmd_type = LWESP_CMD_TYPE_EXECUTE,
	.cmd_key  = "AT+RESTORE",
};

void lwesp_sys_resp_callback(lwesp_resp_t resp) {
	switch(resp) {
		case LWESP_RESP_OK:
			printf("LWESP HAS OK\r\n");
			lwesp_at_resp_flag = LWESP_RESP_OK;
			break;
		case LWESP_RESP_ERR:
			printf("LWESP HAS ERR\r\n");
			lwesp_at_resp_flag = LWESP_RESP_ERR;
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

void lwesp_basic_init(void) {
	lwesp_at_resp_flag = LWESP_RESP_OK;
	lwesp_sys_init(lwesp_sys_resp_callback);
}

lwesp_resp_t lwesp_check_alive(void) {
	
	lwesp_at_resp_flag = LWESP_RESP_UNKNOW;
	lwesp_sys_send_command(basic_at_test_p);
	
	vTaskDelay(10);
	
	return lwesp_at_resp_flag;
}

lwesp_resp_t lwesp_reset_chip(void) {
	
}

lwesp_resp_t lwesp_check_version(void) {

}

lwesp_resp_t lwesp_enter_deep_sleep(uint32_t time_ms) {
	
}

lwesp_resp_t lwesp_set_commands_echo(lwesp_at_echo_t echo) {
	
}

lwesp_resp_t lwesp_restore_chip(void) {
	
}