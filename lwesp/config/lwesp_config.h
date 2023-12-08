// Header: LWESP Header File
// File Name: lwesp.h
// Author: Turgay Hopal
// Date: 28.11.2023


#ifndef INC_LWESP_CONFIG_H_
#define INC_LWESP_CONFIG_H_

// <<< Use Configuration Wizard in Context Menu >>>
//
// <h> LWESP Library Configuration


//   <o>MCU Selection <0=>Disable
//                		<1=>Nuvoton M480 Series
//   <i> Host MCU Selection	
#define LWESP_MCU									 	1

//   <o>UART IT Debug <0=>Disable
//                		<1=>Enable
//   <i> Host MCU Selection	
#define LWESP_UART_IT_DEBUG				 	1


//   <e>Chip Select ESP8266
#define LWESP_CHIP_ESP8266        	0

//   </e>

//   <e>Chip Select ESP32
#define LWESP_CHIP_ESP32	        	1


//   </e>

// </h>

#if LWESP_CHIP_ESP8266 == 1 && LWESP_CHIP_ESP32 == 1
#error "LWESP Library Select One Chip ESP32/ESP8266"
#endif

#if LWESP_MCU == 0
#error "LWESP Library Must Be Selection MCU"
#endif

#if LWESP_MCU == 1
#define LWESP_MCU_NUVOTON_M480
#endif 

// <<< end of configuration section >>>


#endif /* INC_LWESP_CONFIG_H_ */

