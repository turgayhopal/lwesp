// Header: LWESP AT TCP Header File
// File Name: lwesp_at_tcp.h
// Author: Turgay Hopal
// Date: 10.12.2023

#ifndef INC_LWESP_AT_TCP_H_
#define INC_LWESP_AT_TCP_H_

#include "lwesp_typedef.h"
#include "lwesp_sys.h"

void lwesp_at_tcp_focus_on(void);

lwesp_resp_t lwesp_check_conn_status(lwesp_tcp_at_conn_t *conn);
lwesp_resp_t lwesp_resolve_domain(lwesp_tcp_at_domain_t *domain);
lwesp_resp_t lwesp_ping_ip(lwesp_tcp_at_ping_t *ping);
lwesp_resp_t lwesp_set_transmission_mode(lwesp_at_transmission_mode_t mode);
lwesp_resp_t lwesp_set_connection_type(lwesp_at_connection_type_t type);
lwesp_resp_t lwesp_start_connection(lwesp_tcp_at_start_tcp_conn_t start_conn, lwesp_at_connection_type_t type);

#endif /* INC_LWESP_AT_TCP_H_ */