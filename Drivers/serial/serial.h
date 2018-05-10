
#ifndef __serial_H
#define __serial_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include <stdbool.h>
	 
/* uncomment line below for using rs485 mode */
#define SERIAL_RS485_MODE 			1
	 
/* define rts-pin according your hardware */
#ifdef SERIAL_RS485_MODE
	#define USARTx_RTS_PIN				GPIO_PIN_11
	#define USARTx_RTS_GPIO_PORT	GPIOA
#endif
	 
/* define UART_structure according your hardware (huart1, huart2 etc.)*/
extern 		UART_HandleTypeDef		huart1;
	 
#define SERIAL_BUFFERSIZE				1100	
#define SERIAL_PACKAGE_DELAY		1		/* ms */
#define SERIAL_AFTERSEND_DELAY  1 	/* ms */

#define COUNTOF(__BUFFER__)   	(sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
	
typedef struct {
	uint8_t 	RxReg[1];
	uint16_t 	numberRx;		/* count of received bytes */
	uint16_t 	counterRx;	/* counter of received bytes */
	uint32_t 	timerCounter;
	
	uint8_t 	buffer[SERIAL_BUFFERSIZE];
	
	bool			Ready;
} Serial;

extern 		UART_HandleTypeDef huart;

/* USART HAL Callbacks */
void 			HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
void 			HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

/* Serial functions */
void 			Serial_init(void);
uint8_t 	Serial_ready(void);							/* check if received 1 and more bytes and timer ends*/
void 			Serial_rts_toggle(void);   			/* RS485 mode */
void 			Serial_timer_update(void); 			/* upd timer every 1byte received */
void 			Serial_rec1b_enable(void); 			/* enable ISR uart receive */
void 			Serial_rec1b_disable(void); 		/* disable ISR uart receive */
void 			Serial_ready_to_receive(void);	/* clear serial and enable receive */
void 			Serial_send_response(uint8_t * data, uint16_t lenght);
void 			Serial_send_response32(uint32_t * data, uint16_t lenght); /* for 32b array */

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

