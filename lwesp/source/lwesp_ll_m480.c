// Header: LWESP LL Nuvoton M480 Source File
// File Name: lwesp_ll_m480.c
// Author: Turgay Hopal
// Date: 29.11.2023

#include "lwesp_ll_m480.h"

#ifdef USE_M480_DK
static lwesp_ll_uart_t ll_uart_t = {
	.uart_t 										= UART1,
	.baudrate 									= 115200,
	.reset_port									= PH,
	.reset_bit    						  = BIT3,
	.reset_pin                  = &PH3,
	.rx_handler							    = NULL
};
#endif

#ifdef USE_RBOT_ON_DESK
static lwesp_ll_uart_t ll_uart_t = {
	.uart_t 										= UART1,
	.baudrate 									= 115200,
	.reset_port									= PE,
	.reset_bit    						  = BIT10,
	.reset_pin                  = &PE10,
	.rx_handler							    = NULL
};
#endif

void UART1_IRQHandler(void) {
	
	if(ll_uart_t.uart_t->INTSTS % UART_INTSTS_RDAINT_Msk) {
		
		while(UART_IS_RX_READY(ll_uart_t.uart_t))
    {
			uint8_t in_char = UART_READ(ll_uart_t.uart_t);
			ll_uart_t.rx_handler(in_char);
#if LWESP_UART_IT_DEBUG == 1
			printf("%c", in_char);
#endif
		};
	}
}

void lwesp_ll_init(lwesp_ll_t *lwesp_ll) {
	
	lwesp_ll->lwesp_ll_configure_pin = lwesp_ll_configure_pin;
	lwesp_ll->lwesp_ll_configure_uart = lwesp_ll_configure_uart;
	lwesp_ll->lwesp_ll_configure_uart_clock = lwesp_ll_configure_uart_clock;
	lwesp_ll->lwesp_ll_configure_uart_irq = lwesp_ll_configure_uart_irq;
	lwesp_ll->lwesp_ll_hard_reset = lwesp_ll_hard_reset;
	lwesp_ll->lwesp_ll_send_data = lwesp_ll_send_data;
}

lwesp_resp_t lwesp_ll_configure_uart_clock(void) {
	
	lwesp_resp_t resp_t = LWESP_RESP_OK;
	
	SYS_UnlockReg();
	
	if (ll_uart_t.uart_t == UART0) {
		
		CLK->APBCLK0 |= CLK_APBCLK0_UART0CKEN_Msk;
		CLK_EnableModuleClock(UART0_MODULE);
		CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HXT, CLK_CLKDIV0_UART0(1));
		SYS_ResetModule(UART0_RST);
		
	} else if (ll_uart_t.uart_t == UART1) {
		
		CLK->APBCLK0 |= CLK_APBCLK0_UART1CKEN_Msk;
		CLK_EnableModuleClock(UART1_MODULE);
		CLK_SetModuleClock(UART1_MODULE, CLK_CLKSEL1_UART1SEL_HXT, CLK_CLKDIV0_UART1(1));
		SYS_ResetModule(UART1_RST);
		
	} else if (ll_uart_t.uart_t == UART2) {
		
		CLK->APBCLK0 |= CLK_APBCLK0_UART2CKEN_Msk;
		CLK_EnableModuleClock(UART2_MODULE);
		CLK_SetModuleClock(UART2_MODULE, CLK_CLKSEL3_UART2SEL_HXT, CLK_CLKDIV4_UART2(1));
		SYS_ResetModule(UART2_RST);
		
	} else if (ll_uart_t.uart_t == UART3) {
		
		CLK->APBCLK0 |= CLK_APBCLK0_UART3CKEN_Msk;
		CLK_EnableModuleClock(UART3_MODULE);
		CLK_SetModuleClock(UART3_MODULE, CLK_CLKSEL3_UART3SEL_HXT, CLK_CLKDIV4_UART3(1));
		SYS_ResetModule(UART3_RST);
		
	} else if (ll_uart_t.uart_t == UART4) {
		
		CLK->APBCLK0 |= CLK_APBCLK0_UART4CKEN_Msk;
		CLK_EnableModuleClock(UART4_MODULE);
		CLK_SetModuleClock(UART4_MODULE, CLK_CLKSEL3_UART4SEL_HXT, CLK_CLKDIV4_UART4(1));
		SYS_ResetModule(UART4_RST);
		
	} else if (ll_uart_t.uart_t == UART5) {
		
		CLK->APBCLK0 |= CLK_APBCLK0_UART5CKEN_Msk;
		CLK_EnableModuleClock(UART5_MODULE);
		CLK_SetModuleClock(UART5_MODULE, CLK_CLKSEL3_UART5SEL_HXT, CLK_CLKDIV4_UART5(1));
		SYS_ResetModule(UART5_RST);
		
	} else if (ll_uart_t.uart_t == UART6) {
		
		CLK->APBCLK0 |= CLK_APBCLK0_UART6CKEN_Msk;
		CLK_EnableModuleClock(UART6_MODULE);
		CLK_SetModuleClock(UART6_MODULE, CLK_CLKSEL3_UART5SEL_HXT, CLK_CLKDIV4_UART6(1));
		SYS_ResetModule(UART6_RST);
		
	} else if (ll_uart_t.uart_t == UART7) {
		
		CLK->APBCLK0 |= CLK_APBCLK0_UART7CKEN_Msk;
		CLK_EnableModuleClock(UART7_MODULE);
		CLK_SetModuleClock(UART7_MODULE, CLK_CLKSEL3_UART7SEL_HXT, CLK_CLKDIV4_UART7(1));
		SYS_ResetModule(UART7_RST);
		
	} else {
		resp_t = LWESP_RESP_CONF_ERR;
	}
	
	SYS_LockReg();
	
	return resp_t;
}

lwesp_resp_t lwesp_ll_configure_pin(void) {
	
#ifdef USE_M480_DK
	SYS->GPH_MFPH &= ~(SYS_GPH_MFPH_PH8MFP_Msk | SYS_GPH_MFPH_PH9MFP_Msk);
	SYS->GPH_MFPH |= (SYS_GPH_MFPH_PH8MFP_UART1_TXD | SYS_GPH_MFPH_PH9MFP_UART1_RXD);
#endif
	
#ifdef USE_RBOT_ON_DESK
	SYS->GPC_MFPH &= ~(SYS_GPC_MFPH_PC15MFP_Msk);
	SYS->GPC_MFPH |= (SYS_GPC_MFPH_PC8MFP_UART1_RXD);
	
	SYS->GPE_MFPH &= ~(SYS_GPE_MFPH_PE11MFP_Msk | SYS_GPE_MFPH_PE12MFP_Msk | SYS_GPE_MFPH_PE13MFP_Msk);
	SYS->GPE_MFPH |= (SYS_GPE_MFPH_PE11MFP_UART1_nCTS | SYS_GPE_MFPH_PE12MFP_UART1_nRTS | SYS_GPE_MFPH_PE13MFP_UART1_TXD);
#endif
	
	return LWESP_RESP_OK;
}

lwesp_resp_t lwesp_ll_configure_uart(void) {
	UART_Open(ll_uart_t.uart_t, ll_uart_t.baudrate);
	UART_DisableFlowCtrl(ll_uart_t.uart_t);
	
	return LWESP_RESP_OK;
}

lwesp_resp_t lwesp_ll_configure_uart_irq(void) {
	
	lwesp_resp_t resp_t = LWESP_RESP_OK;
	
	UART_EnableInt(ll_uart_t.uart_t, UART_INTEN_RDAIEN_Msk | UART_INTEN_RXTOIEN_Msk);	
	
	if (ll_uart_t.uart_t == UART0) {
		NVIC_EnableIRQ(UART0_IRQn);
	} else if (ll_uart_t.uart_t == UART1) {
		NVIC_EnableIRQ(UART1_IRQn);
	} else if (ll_uart_t.uart_t == UART2) {
		NVIC_EnableIRQ(UART2_IRQn);
	} else if (ll_uart_t.uart_t == UART3) {
		NVIC_EnableIRQ(UART3_IRQn);
	} else if (ll_uart_t.uart_t == UART4) {
		NVIC_EnableIRQ(UART4_IRQn);
	} else if (ll_uart_t.uart_t == UART5) {
		NVIC_EnableIRQ(UART5_IRQn);
	} else if (ll_uart_t.uart_t == UART6) {
		NVIC_EnableIRQ(UART6_IRQn);
	} else if (ll_uart_t.uart_t == UART7) {
		NVIC_EnableIRQ(UART7_IRQn);
	} else {
		resp_t = LWESP_RESP_CONF_ERR;
	}
	
	return resp_t;
}

lwesp_resp_t lwesp_ll_hard_reset(uint8_t state) {
		
	GPIO_SetMode(ll_uart_t.reset_port, ll_uart_t.reset_bit, GPIO_MODE_QUASI);
	*ll_uart_t.reset_pin = ~state;
	
	return LWESP_RESP_OK;
}

lwesp_resp_t lwesp_ll_send_data(uint8_t *data, uint32_t size) {
	
	UART_Write(ll_uart_t.uart_t, data, size);
	UART_Write(ll_uart_t.uart_t, (uint8_t *)"\r\n", 2);
	
	return LWESP_RESP_OK;
}

lwesp_resp_t lwesp_ll_configure_rx_callback(lwesp_ll_uart_rx_handler rx_handler) {
	ll_uart_t.rx_handler = rx_handler;
}
