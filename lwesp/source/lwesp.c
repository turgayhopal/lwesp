// Header: LWESP Source File
// File Name: lwesp.c
// Author: Turgay Hopal
// Date: 29.11.2023

#include "lwesp.h"

lwesp_resp_t lwesp_init(lwesp_client_t *client) {
	
	client->basic.lwesp_check_alive = lwesp_check_alive;
	client->basic.lwesp_check_version = lwesp_check_version;
	client->basic.lwesp_enter_deep_sleep = lwesp_enter_deep_sleep;
	client->basic.lwesp_reset_chip = lwesp_reset_chip;
	client->basic.lwesp_restore_chip = lwesp_restore_chip;
	client->basic.lwesp_set_commands_echo = lwesp_set_commands_echo;
	client->basic.lwesp_set_sleep_mode = lwesp_set_sleep_mode;
	client->basic.lwesp_check_sleep_mode = lwesp_check_sleep_mode;
	client->basic.lwesp_set_rf_power = lwesp_set_rf_power;
	
	lwesp_ll_t lwesp_ll;
	lwesp_ll_init(&lwesp_ll);

	lwesp_basic_init();
	
	lwesp_resp_t resp;
	
	resp |= lwesp_ll.lwesp_ll_configure_uart_clock();
	resp |= lwesp_ll.lwesp_ll_configure_pin();
	resp |= lwesp_ll.lwesp_ll_configure_uart();
	resp |= lwesp_ll.lwesp_ll_configure_uart_irq();
		
	return resp;
}