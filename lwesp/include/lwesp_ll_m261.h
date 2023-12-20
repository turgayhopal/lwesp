// Header: LWESP Low Level Nuvoton M261 Header File
// File Name: lwesp_ll_m261.h
// Author: Turgay Hopal
// Date: 20.12.2023

#ifndef INC_LWESP_LL_M261_H_
#define INC_LWESP_LL_M261_H_

#include "lwesp_typedef.h"

#ifdef LWESP_MCU_NUVOTON_M261

#include "M261.h"

typedef void (*lwesp_ll_uart_rx_handler)(uint8_t rx_data);

typedef struct lwesp_ll_uart_s {
	UART_T 							*uart_t;
	uint32_t 						baudrate;
	GPIO_T 		  				*reset_port;
	uint32_t 						reset_bit;
	volatile uint32_t   *reset_pin;							
	lwesp_ll_uart_rx_handler rx_handler;	
} lwesp_ll_uart_t;

void lwesp_ll_init(lwesp_ll_t *lwesp_ll);

lwesp_resp_t lwesp_ll_configure_uart_clock(void);
lwesp_resp_t lwesp_ll_configure_pin(void);
lwesp_resp_t lwesp_ll_configure_uart(void);
lwesp_resp_t lwesp_ll_configure_uart_irq(void);
lwesp_resp_t lwesp_ll_hard_reset(uint8_t state);
lwesp_resp_t lwesp_ll_send_data(uint8_t *data, uint32_t size);
lwesp_resp_t lwesp_ll_configure_rx_callback(lwesp_ll_uart_rx_handler rx_handler);

#endif

#endif /* INC_LWESP_LL_M261_H_ */