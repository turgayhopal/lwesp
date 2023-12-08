// Header: LWESP System Control Source File
// File Name: lwesp_sys.c
// Author: Turgay Hopal
// Date: 29.11.2023

#include "lwesp_sys.h"

static lwesp_queue_t lwesp_queue_rx;

static lwesp_sys_t lwesp_sys = {
	.resp_callback = NULL,
	.resp_wifi_callback = NULL
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
			
			if (strstr((char *)lwesp_queue_rx.data, "OK") != NULL) {
				memset(lwesp_response_buffer, 0x00, LWESP_SYS_RESP_BUFFER_SIZE);
				sprintf((char *)lwesp_response_buffer, "%s", lwesp_queue_rx.data);
				lwesp_queue_flush(&lwesp_queue_rx);
				lwesp_sys.resp_callback(LWESP_RESP_OK);
			}
			
			if (strstr((char *)lwesp_queue_rx.data, "ERROR") != NULL ) {
				lwesp_queue_flush(&lwesp_queue_rx);
				lwesp_sys.resp_callback(LWESP_RESP_ERR);
			}
						
			if (strstr((char *)lwesp_queue_rx.data, "WIFI DISCONNECT") != NULL ) {
				lwesp_queue_flush(&lwesp_queue_rx);
				lwesp_sys.resp_wifi_callback(LWESP_RESP_WIFI_DISCONNECT, NULL);
			}
			
			if (strstr((char *)lwesp_queue_rx.data, "WIFI CONNECTED") != NULL ) {
				lwesp_queue_flush(&lwesp_queue_rx);
				lwesp_sys.resp_wifi_callback(LWESP_RESP_WIFI_CONNECTED, NULL);
			}
			
			if (strstr((char *)lwesp_queue_rx.data, "WIFI GOT IP") != NULL ) {
				lwesp_queue_flush(&lwesp_queue_rx);
				lwesp_sys.resp_wifi_callback(LWESP_RESP_WIFI_GOT_IP, NULL);
			}
			vTaskDelay(1);
		}
	}
}

void lwesp_sys_init(void) {
		
	lwesp_queue_init(&lwesp_queue_rx);
	
	lwesp_ll_t lwesp_ll;
	lwesp_ll_init(&lwesp_ll);
	
	lwesp_ll.lwesp_ll_configure_uart_clock();
	lwesp_ll.lwesp_ll_configure_pin();
	lwesp_ll.lwesp_ll_configure_uart();
	lwesp_ll.lwesp_ll_configure_uart_irq();
	lwesp_ll_configure_rx_callback(lwesp_ll_rx_handler_callback);
	
	xTaskCreate(vRxHandlerTask, "LWESP RX Handler",  lwespRX_HANDLER_TASK_STACK_SIZE, NULL, lwespRX_HANDLER_TASK_PRIORITY, NULL);
	
}

void lwesp_sys_set_resp_callback(lwesp_resp_callback resp_callback) {
	lwesp_sys.resp_callback = resp_callback;
}

void lwesp_sys_set_resp_wifi_callback(lwesp_resp_wifi_callback resp_callback) {
	lwesp_sys.resp_wifi_callback = resp_callback;
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

void lwesp_sys_at_get_wifi_mode(lwesp_wifi_at_wifi_mode_t *wifi_mode, uint8_t save_flash_st) {
	char* token;
	char* rest = (char *)lwesp_response_buffer;
	
	while ((token = strtok_r(rest, "\n", &rest))) {
		char* key = strtok(token, ":");
		char* value = strtok(NULL, ":");

		if (key != NULL && value != NULL) {
			if (save_flash_st) {
				if (strstr(key, "+CWMODE_DEF") != NULL) {
					sprintf((char *)wifi_mode->wifi_mode, "%s", value);
				}
			} else {
				if (strstr(key, "+CWMODE_CUR") != NULL) {
					sprintf((char *)wifi_mode->wifi_mode, "%s", value);
				}
			}
		}
	}
}

void lwesp_sys_at_get_list_ap(void) {

	char* token;
	char* rest = (char *)lwesp_response_buffer;
	
	while ((token = strtok_r(rest, "\n", &rest))) {
		char* key = strtok(token, "(");
		char* value = strtok(NULL, "(");
		
		if (key != NULL && value != NULL) {
			
			int len = strlen(value);
			int i, j;

			for (i = 0, j = 0; i < len; i++) {
					if (value[i] != ')') {
							value[j++] = value[i];
					}
			}
			value[j] = '\0';
			
			lwesp_sys.resp_wifi_callback(LWESP_RESP_WIFI_AP_FOUND, value);
		}
	}
	
}


void lwesp_sys_send_command(lwesp_at_parameter_t parameter) {
	
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

