// Header: LWESP AT Basic Header File
// File Name: lwesp_at_basic.h
// Author: Turgay Hopal
// Date: 28.11.2023

#ifndef INC_LWESP_AT_BASIC_H_
#define INC_LWESP_AT_BASIC_H_

#include "lwesp_typedef.h"
#include "lwesp_sys.h"

void lwesp_at_basic_focus_on(void);

lwesp_resp_t lwesp_check_alive(void);

lwesp_resp_t lwesp_reset_chip(void);

lwesp_resp_t lwesp_check_version(lwesp_basic_at_version_t *at_version_t);

lwesp_resp_t lwesp_enter_deep_sleep(uint32_t time_ms);

lwesp_resp_t lwesp_set_commands_echo(lwesp_at_echo_t echo);

lwesp_resp_t lwesp_restore_chip(void);

lwesp_resp_t lwesp_set_sleep_mode(lwesp_basic_at_sleep_mode_t sleep_mode);

lwesp_resp_t lwesp_check_sleep_mode(lwesp_basic_at_sleep_mode_t *sleep_mode);

lwesp_resp_t lwesp_set_rf_power(lwesp_basic_at_rf_power_t rf_power);

#endif /* INC_LWESP_AT_BASIC_H_ */