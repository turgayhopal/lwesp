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
			
			if (strstr((char *)lwesp_queue_rx.data, "+STA_CONNECTED") != NULL ) {
				lwesp_queue_flush(&lwesp_queue_rx);
				lwesp_sys.resp_wifi_callback(LWESP_RESP_WIFI_STA_CONNECTED, NULL);
			}
			
			if (strstr((char *)lwesp_queue_rx.data, "+STA_DISCONNECTED") != NULL ) {
				lwesp_queue_flush(&lwesp_queue_rx);
				lwesp_sys.resp_wifi_callback(LWESP_RESP_WIFI_STA_DISCONNECTED, NULL);
			}
			
			if (strstr((char *)lwesp_queue_rx.data, "+DIST_STA_IP") != NULL ) {
				lwesp_queue_flush(&lwesp_queue_rx);
				lwesp_sys.resp_wifi_callback(LWESP_RESP_WIFI_STA_GET_IP, NULL);
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

#if LWESP_CHIP_ESP32 == 1
void lwesp_sys_at_get_wifi_mode(lwesp_wifi_at_wifi_mode_t *wifi_mode) {
	
	char* token;
	char* rest = (char *)lwesp_response_buffer;
	
	while ((token = strtok_r(rest, "\n", &rest))) {
		char* key = strtok(token, ":");
		char* value = strtok(NULL, ":");

		if (key != NULL && value != NULL) {
			if (strstr(key, "+CWMODE") != NULL) {
				sprintf((char *)wifi_mode->wifi_mode, "%s", value);
			}
		}
	}
}

void lwesp_sys_at_get_mac_ap(lwesp_wifi_at_mac_t *mac_addr) {

	char* rest = (char *)lwesp_response_buffer;
	
	char* mac_start = strstr(rest, "+CIPAPMAC:");
	if (mac_start != NULL) {
		if (sscanf(mac_start, "+CIPAPMAC:\"%17[^\"\n]", mac_addr->mac_addr) == 1) {
			// 
		} 
	}
}

void lwesp_sys_at_get_mac_sta(lwesp_wifi_at_mac_t *mac_addr) {

	char* rest = (char *)lwesp_response_buffer;
	
	char* mac_start = strstr(rest, "+CIPSTAMAC:");
	if (mac_start != NULL) {
		if (sscanf(mac_start, "+CIPSTAMAC:\"%17[^\"\n]", mac_addr->mac_addr) == 1) {
			// 
		} 
	}
}

void lwesp_sys_at_get_ip_ap(lwesp_wifi_at_ip_t *ip_addr) {
		
	char *current = (char *)lwesp_response_buffer;
    
	while ((current = strstr(current, "CIPAP:")) != NULL) {
		if (strstr(current, "ip:") != NULL) {
			if (sscanf(current, "CIPAP:ip:\"%15[^\"]\"", ip_addr->ip) != 1) {
				//
			}
		} else if (strstr(current, "gateway:") != NULL) {
			if (sscanf(current, "CIPAP:gateway:\"%15[^\"]\"", ip_addr->gateway) != 1) {
				//
			}
		} else if (strstr(current, "netmask:") != NULL) {
			if (sscanf(current, "CIPAP:netmask:\"%15[^\"]\"", ip_addr->netmask) != 1) {
				//
			}
		}
		current++;
	}
	
}


void lwesp_sys_at_get_ip_sta(lwesp_wifi_at_ip_t *ip_addr) {
	
	char *current = (char *)lwesp_response_buffer;
    
	while ((current = strstr(current, "CIPSTA:")) != NULL) {
		if (strstr(current, "ip:") != NULL) {
			if (sscanf(current, "CIPSTA:ip:\"%15[^\"]\"", ip_addr->ip) != 1) {
				//
			}
		} else if (strstr(current, "gateway:") != NULL) {
			if (sscanf(current, "CIPSTA:gateway:\"%15[^\"]\"", ip_addr->gateway) != 1) {
				//
			}
		} else if (strstr(current, "netmask:") != NULL) {
			if (sscanf(current, "CIPSTA:netmask:\"%15[^\"]\"", ip_addr->netmask) != 1) {
				//
			}
		}
		current++;
	}
	
}

#endif

#if LWESP_CHIP_ESP8266 == 1
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

void lwesp_sys_at_get_mac_ap(lwesp_wifi_at_mac_t *mac_addr, uint8_t save_flash_st) {

	char* rest = (char *)lwesp_response_buffer;

	if (save_flash_st) {
		char* mac_start = strstr(rest, "+CIPAPMAC_DEF:");
		if (mac_start != NULL) {
			if (sscanf(mac_start, "+CIPAPMAC_DEF:\"%17[^\"\n]", mac_addr->mac_addr) == 1) {
				// 
			} 
		} 
	} else {
		char* mac_start = strstr(rest, "+CIPAPMAC_CUR:");
		if (mac_start != NULL) {
			if (sscanf(mac_start, "+CIPAPMAC_CUR:\"%17[^\"\n]", mac_addr->mac_addr) == 1) {
				// 
			} 
		}
	}

}

void lwesp_sys_at_get_mac_sta(lwesp_wifi_at_mac_t *mac_addr, uint8_t save_flash_st) {

	char* rest = (char *)lwesp_response_buffer;
	
	if (save_flash_st) {
		char* mac_start = strstr(rest, "+CIPSTAMAC_DEF:");
		if (mac_start != NULL) {
			if (sscanf(mac_start, "+CIPSTAMAC_DEF:\"%17[^\"\n]", mac_addr->mac_addr) == 1) {
				// 
			} 
		} 
	} else {
		char* mac_start = strstr(rest, "+CIPSTAMAC_CUR:");
		if (mac_start != NULL) {
			if (sscanf(mac_start, "+CIPSTAMAC_CUR:\"%17[^\"\n]", mac_addr->mac_addr) == 1) {
				// 
			} 
		}
	}
}

void lwesp_sys_at_get_ip_ap(lwesp_wifi_at_ip_t *ip_addr, uint8_t save_flash_st) {
	
	char *current = (char *)lwesp_response_buffer;
    
	while ((current = strstr(current, "CIPAP_CUR:")) != NULL) {
		if (strstr(current, "ip:") != NULL) {
			if (save_flash_st) {
				if (sscanf(current, "CIPAP_DEF:ip:\"%15[^\"]\"", ip_addr->ip) != 1) {
				//
				}
			} else {
				if (sscanf(current, "CIPAP_CUR:ip:\"%15[^\"]\"", ip_addr->ip) != 1) {
				//
				}
			}
		} else if (strstr(current, "gateway:") != NULL) {
			if (save_flash_st) {
				if (sscanf(current, "CIPAP_DEF:gateway:\"%15[^\"]\"", ip_addr->gateway) != 1) {
				//
				}
			} else {
				if (sscanf(current, "CIPAP_CUR:gateway:\"%15[^\"]\"", ip_addr->gateway) != 1) {
				//
				}
			}
		} else if (strstr(current, "netmask:") != NULL) {
			if (save_flash_st) {
				if (sscanf(current, "CIPAP_DEF:netmask:\"%15[^\"]\"", ip_addr->netmask) != 1) {
				//
				}
			} else {
				if (sscanf(current, "CIPAP_CUR:netmask:\"%15[^\"]\"", ip_addr->netmask) != 1) {
				//
				}
			}
		}
		current++;
	}
	
}

void lwesp_sys_at_get_ip_sta(lwesp_wifi_at_ip_t *ip_addr, uint8_t save_flash_st) {
	
	char *current = (char *)lwesp_response_buffer;
    
	while ((current = strstr(current, "CIPSTA_CUR:")) != NULL) {
		if (strstr(current, "ip:") != NULL) {
			if (save_flash_st) {
				if (sscanf(current, "CIPSTA_DEF:ip:\"%15[^\"]\"", ip_addr->ip) != 1) {
				//
				}
			} else {
				if (sscanf(current, "CIPSTA_CUR:ip:\"%15[^\"]\"", ip_addr->ip) != 1) {
				//
				}
			}
		} else if (strstr(current, "gateway:") != NULL) {
			if (save_flash_st) {
				if (sscanf(current, "CIPSTA_DEF:gateway:\"%15[^\"]\"", ip_addr->gateway) != 1) {
				//
				}
			} else {
				if (sscanf(current, "CIPSTA_CUR:gateway:\"%15[^\"]\"", ip_addr->gateway) != 1) {
				//
				}
			}
		} else if (strstr(current, "netmask:") != NULL) {
			if (save_flash_st) {
				if (sscanf(current, "CIPSTA_DEF:netmask:\"%15[^\"]\"", ip_addr->netmask) != 1) {
				//
				}
			} else {
				if (sscanf(current, "CIPSTA_CUR:netmask:\"%15[^\"]\"", ip_addr->netmask) != 1) {
				//
				}
			}
		}
		current++;
	}

}
#endif

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

void lwesp_sys_at_check_soft_ap_devices(void) {
		
	char* rest = (char *)lwesp_response_buffer;

#if LWESP_CHIP_ESP8266 == 1
	char* token;
	
	while ((token = strtok_r(rest, "\n", &rest))) {
	
		char* key = strtok(token, ",");
		char* value = strtok(NULL, ",");
		
		if (key != NULL && value != NULL) {
			
			char data[50];
			sprintf(data, "%s,%s", key, value);
			lwesp_sys.resp_wifi_callback(LWESP_RESP_WIFI_STA_GET_IP, data);
		}
	}
#endif

#if LWESP_CHIP_ESP32 == 1
		
		char ip_address[20], mac_address[20];
    sscanf(rest, "+CWLIF:%[^,],%s", ip_address, mac_address);
		char data[50];
		sprintf(data, "%s,%s", ip_address, mac_address);
		lwesp_sys.resp_wifi_callback(LWESP_RESP_WIFI_STA_GET_IP, data);
		
#endif

}

void lwesp_sys_at_get_conn_stat(lwesp_tcp_at_conn_t *conn) {

	char* rest = (char *)lwesp_response_buffer;
		
#if LWESP_CHIP_ESP8266 == 1
	sscanf(rest, "STATUS:%s", conn->stat);
#endif
	sscanf(rest, "+CIPSTATE:%s,\"%[^\"]\",\"%[^\"]\",%s,%s,\"%[^\"]\"", conn->link_id, conn->type, conn->remote_ip, conn->remote_port, conn->local_port, conn->type);
	
}

void lwesp_sys_at_get_domain_name(lwesp_tcp_at_domain_t *domain) {
	
	if (sscanf((char *)lwesp_response_buffer, "+CIPDOMAIN:%s", domain->domain_ip) == 1) {
			//
	} 
}

void lwesp_sys_at_get_ping_time(lwesp_tcp_at_ping_t *ping) {
		
	#if LWESP_CHIP_ESP8266 == 1
	if (sscanf((char *)lwesp_response_buffer, "+%s", ping->time) == 1) {
			//
	} 

	#endif
	#if LWESP_CHIP_ESP32 == 1
	if (sscanf((char *)lwesp_response_buffer, "+PING:%s", ping->time) == 1) {
			//
	} 
	#endif
}

lwesp_resp_tcp_t lwesp_sys_at_check_tcp_connection(void) {
	if (strstr((char *)lwesp_response_buffer, "CONNECT") != NULL ) {
		return LWESP_RESP_CONNECT;
	}
	
	if (strstr((char *)lwesp_response_buffer, "ALREADY CONNECT") != NULL ) {
		return LWESP_RESP_ALREADY_CONNECT;
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

