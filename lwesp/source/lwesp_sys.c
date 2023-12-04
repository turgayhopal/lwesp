// Header: LWESP System Control Source File
// File Name: lwesp_sys.c
// Author: Turgay Hopal
// Date: 29.11.2023

#include "lwesp_sys.h"

static lwesp_queue_t lwesp_queue_rx;

static lwesp_sys_t lwesp_sys = {
	.resp_callback = NULL,
};


uint8_t lwesp_sys_tx_buffer[LWESP_SYS_TX_BUFFER_SIZE];
uint8_t lwesp_sys_rx_buffer[LWESP_SYS_RX_BUFFER_SIZE];

void lwesp_ll_rx_handler_callback(uint8_t data) {
	if (lwesp_queue_is_full(&lwesp_queue_rx)) {
		lwesp_queue_pop(&lwesp_queue_rx);
	}
	lwesp_queue_push(&lwesp_queue_rx, data);
}

void vRxHandlerTask(void *pvParameters) {
	/* Task Setup */

	/* Task Loop */

	for (;;) {
		
		if (!lwesp_queue_is_empty(&lwesp_queue_rx)) {
			if (strstr((char *)lwesp_queue_rx.data, "OK")) {
				lwesp_queue_flush(&lwesp_queue_rx);
				lwesp_sys.resp_callback(LWESP_RESP_OK);
			}
			if (strstr((char *)lwesp_queue_rx.data, "ERROR")) {
				lwesp_queue_flush(&lwesp_queue_rx);
				lwesp_sys.resp_callback(LWESP_RESP_ERR);
			}
		}
		
		vTaskDelay(1);
		
	}
}

void lwesp_sys_init(lwesp_resp_callback resp_callback) {
	
	lwesp_sys.resp_callback = resp_callback;
	
	lwesp_queue_init(&lwesp_queue_rx);
	lwesp_ll_configure_rx_callback(lwesp_ll_rx_handler_callback);
	
	xTaskCreate(vRxHandlerTask, "LWESP RX Handler",  lwespRX_HANDLER_TASK_STACK_SIZE, NULL, lwespRX_HANDLER_TASK_PRIORITY, NULL);
	
}

void lwesp_sys_send_command(lwesp_at_parameter_t parameter) {
	
	memset(lwesp_sys_tx_buffer, 0x00, LWESP_SYS_TX_BUFFER_SIZE);
	
	switch(parameter.cmd_type) {
		case LWESP_CMD_TYPE_EXECUTE:
			sprintf((char *) lwesp_sys_tx_buffer, "%s", parameter.cmd_key);
			lwesp_ll_send_data(lwesp_sys_tx_buffer, strlen((char *)lwesp_sys_tx_buffer));
			break;
		case LWESP_CMD_TYPE_QUERY:
			break;
		case LWESP_CMD_TYPE_SET:
			break;
		case LWESP_CMD_TYPE_TEST:
			break;
	}
	
}