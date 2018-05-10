
#ifndef __parser_H
#define __parser_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f1xx_hal.h"
	 
/* Protocol settings */
#define CMD_NUMB_START_BYTES		2
#define CMD_NUMB_ADDR_BYTES			1
#define CMD_NUMB_FLAG_BYTES			1
#define CMD_NUMB_DATA_BYTES			2
#define CMD_NUMB_CS_BYTES				1
// 0xBB 0xBB Addr Cmd D1 D2 CS = 7
	
#define CMD_NUBM_BYTES					7
#define CMD_NUBM_PROTOCOL_BYTES	5 //CMD_NUBM_BYTES - CMD_NUMB_DATA_BYTES

#define START_BYTE 							0xBB	 
#define RS485_ADDR_DEF 					0x00

uint8_t 	Parser_check_request(uint8_t * msg, uint16_t len);
uint16_t 	Parser_GetData16(uint8_t * msg);

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

