// Header: LWESP System Control Header File
// File Name: lwesp_sys.h
// Author: Turgay Hopal
// Date: 28.11.2023


#ifndef INC_LWESP_SYS_H_
#define INC_LWESP_SYS_H_

#include "lwesp_typedef.h"

#define LWESP_SYS_TX_BUFFER_SIZE		1024
#define LWESP_SYS_RESP_BUFFER_SIZE	1024

#define lwespRX_HANDLER_TASK_PRIORITY				(tskIDLE_PRIORITY + 3UL)
#define lwespRX_HANDLER_TASK_STACK_SIZE			(configMINIMAL_STACK_SIZE)

#ifdef LWESP_MCU_NUVOTON_M480
#include "lwesp_ll_m480.h"
#endif

typedef void (*lwesp_resp_callback)(lwesp_resp_t resp);

typedef struct lwesp_sys_s {
	lwesp_resp_callback resp_callback;
	char resp_ok_key[20];
	char resp_err_key[20];
}lwesp_sys_t;

void vRxHandlerTask(void *pvParameters);

void lwesp_sys_init(lwesp_resp_callback resp_callback);
void lwesp_sys_send_command(lwesp_at_parameter_t parameter);

void lwesp_sys_at_get_version(lwesp_basic_at_version_t *at_version);
void lwesp_sys_at_get_sleep_mode(lwesp_basic_at_sleep_mode_t *sleep_mode);

#endif /* INC_LWESP_SYS_H_ */