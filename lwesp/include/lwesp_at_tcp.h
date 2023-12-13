
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

#if LWESP_CHIP_ESP8266 == 1
lwesp_resp_t lwesp_set_ssl_size(lwesp_tcp_at_set_ssl_size_t ssl);
#endif

lwesp_resp_t lwesp_start_tcp_connection(lwesp_tcp_at_start_tcp_conn_t start_conn, lwesp_at_connection_type_t type);
lwesp_resp_t lwesp_start_udp_connection(lwesp_tcp_at_start_udp_conn_t start_conn, lwesp_at_connection_type_t type);
lwesp_resp_t lwesp_start_ssl_connection(lwesp_tcp_at_start_ssl_conn_t start_conn, lwesp_at_connection_type_t type);
lwesp_resp_t lwesp_close_connection(uint8_t *link_id, lwesp_at_connection_type_t type);
lwesp_resp_t lwesp_send_data_lenght(lwesp_tcp_at_send_data_t send_data, uint8_t *link_id, lwesp_at_connection_type_t type);
lwesp_resp_t lwesp_send_data(lwesp_tcp_at_send_data_t send_data, char *response_body, int *status_code);
lwesp_resp_t lwesp_get_ip_addr(lwesp_tcp_at_get_ip_addr_t *ip);
lwesp_resp_t lwesp_create_tcp_server(lwesp_tcp_at_create_tcp_server_t server);
lwesp_resp_t lwesp_set_tcp_server_timeout(lwesp_tcp_at_set_tcp_server_timeout_t timeout);

#endif /* INC_LWESP_AT_TCP_H_ */