// Header: LWESP System Control Source File
// File Name: lwesp_sys.c
// Author: Turgay Hopal
// Date: 29.11.2023

#include "lwesp_sys.h"

static lwesp_queue_t lwesp_queue_rx;

static lwesp_sys_t lwesp_sys = {
	.resp_callback = NULL,
	.resp_ok_key = NULL,
	.resp_err_key = NULL
};

uint8_t lwesp_sys_tx_buffer[LWESP_SYS_TX_BUFFER_SIZE];

static uint8_t lwesp_response_buffer[LWESP_SYS_RESP_BUFFER_SIZE];

void lwesp_ll_rx_handler_callback(uint8_t data) {
	if (lwesp_queue_is_full(&lwesp_queue_rx)) {
		lwesp_queue_pop(&lwesp_queue_rx);
	}
	lwesp_queue_push(&lwesp_queue_rx, data);
}

void vRxHandlerTask(void *pvParameters) {
	/* Task Setup */
	
	memset(lwesp_response_buffer, 0x00, LWESP_SYS_RESP_BUFFER_SIZE);
	
	/* Task Loop */

	for (;;) {
		
		if (!lwesp_queue_is_empty(&lwesp_queue_rx)) {
			
			if (strstr((char *)lwesp_queue_rx.data, lwesp_sys.resp_ok_key) != NULL) {
				memset(lwesp_response_buffer, 0x00, LWESP_SYS_RESP_BUFFER_SIZE);
				sprintf((char *)lwesp_response_buffer, "%s", lwesp_queue_rx.data);
				lwesp_queue_flush(&lwesp_queue_rx);
				lwesp_sys.resp_callback(LWESP_RESP_OK);
			}
			
			if (strstr((char *)lwesp_queue_rx.data, lwesp_sys.resp_err_key) != NULL ) {
				lwesp_queue_flush(&lwesp_queue_rx);
				lwesp_sys.resp_callback(LWESP_RESP_ERR);
			}
			vTaskDelay(1);
		}
	}
}

void lwesp_sys_init(lwesp_resp_callback resp_callback) {
	
	lwesp_sys.resp_callback = resp_callback;
	
	lwesp_queue_init(&lwesp_queue_rx);
	lwesp_ll_configure_rx_callback(lwesp_ll_rx_handler_callback);
	
	xTaskCreate(vRxHandlerTask, "LWESP RX Handler",  lwespRX_HANDLER_TASK_STACK_SIZE, NULL, lwespRX_HANDLER_TASK_PRIORITY, NULL);
	
}

void lwesp_sys_at_get_version(lwesp_basic_at_version_t *at_version) {
	
	char* token;
	char* rest = (char *)lwesp_response_buffer;
	
	while ((token = strtok_r(rest, "\n", &rest))) {
		char* key = strtok(token, ":");
		char* value = strtok(NULL, ":");

		if (key != NULL && value != NULL) {
			if (strstr(key, "AT version") != NULL) {
				sprintf((char *)at_version->at_version_info, "%s", value);
			}
			if (strstr(key, "SDK version") != NULL) {
				sprintf((char *)at_version->sdk_version_info, "%s", value);
			}
			if (strstr(key, "Bin version") != NULL) {
				sprintf((char *)at_version->bin_version, "%s", value);
			}
		}
	}
}

void lwesp_sys_at_get_sleep_mode(lwesp_basic_at_sleep_mode_t *sleep_mode) {
	char* token;
	char* rest = (char *)lwesp_response_buffer;
	
	while ((token = strtok_r(rest, "\n", &rest))) {
		char* key = strtok(token, ":");
		char* value = strtok(NULL, ":");

		if (key != NULL && value != NULL) {
			if (strstr(key, "+SLEEP") != NULL) {
				sprintf((char *)sleep_mode->sleep_mode, "%s", value);
			}
		}
	}
}

void lwesp_sys_send_command(lwesp_at_parameter_t parameter) {
	
	sprintf(lwesp_sys.resp_ok_key, "%s", parameter.resp_ok_key);
	sprintf(lwesp_sys.resp_err_key, "%s", parameter.resp_err_key);
	
	memset(lwesp_sys_tx_buffer, 0x00, LWESP_SYS_TX_BUFFER_SIZE);
	
	switch(parameter.cmd_type) {
		case LWESP_CMD_TYPE_EXECUTE:
			sprintf((char *) lwesp_sys_tx_buffer, "%s", parameter.cmd_key);
			break;
		case LWESP_CMD_TYPE_QUERY:
			sprintf((char *) lwesp_sys_tx_buffer, "%s?", parameter.cmd_key);
			break;
		case LWESP_CMD_TYPE_SET:
			sprintf((char *) lwesp_sys_tx_buffer, "%s=%s", parameter.cmd_key, parameter.cmd_params);
			break;
		case LWESP_CMD_TYPE_TEST:
			break;
	}
	lwesp_ll_send_data(lwesp_sys_tx_buffer, strlen((char *)lwesp_sys_tx_buffer));
}