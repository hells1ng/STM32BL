
#include "serial.h"

UART_HandleTypeDef 	huart;
Serial 							serial;

void Serial_init(void)
{
	serial.RxReg[0] 		= 0;
	serial.numberRx 		= 0;
	serial.counterRx 		= 0;
	serial.timerCounter = 0;
	
	serial.Ready 				= false;
}

void Serial_rts_toggle(void)
{
#ifdef SERIAL_RS485_MODE
	HAL_GPIO_TogglePin(USARTx_RTS_GPIO_PORT, USARTx_RTS_PIN);
#endif
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	Serial_rts_toggle();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	serial.buffer[serial.counterRx] = serial.RxReg[0];
	serial.counterRx ++;
	
	Serial_timer_update();
	
	Serial_rec1b_enable();
}


void Serial_timer_update(void)
{
	serial.timerCounter = HAL_GetTick();
}


void Serial_rec1b_enable(void)
{
	HAL_UART_Receive_IT(&huart1, (uint8_t *)serial.RxReg, 1);
}
void Serial_rec1b_disable(void)
{
	HAL_UART_AbortReceive(&huart1);
}
void Serial_send_response(uint8_t * data, uint16_t lenght)
{
	Serial_rts_toggle();
	HAL_UART_Transmit_DMA(&huart1, data, lenght);
	HAL_Delay(SERIAL_AFTERSEND_DELAY);
}

void Serial_ready_to_receive(void)
{	
	serial.numberRx = 0;
	serial.counterRx = 0;
	
	Serial_rec1b_enable();
}


void Serial_send_response32(uint32_t * data, uint16_t lenght)
{

	for (uint16_t i = 0; i < (lenght * 4); i = i + 4)
	{		
		//clean
		serial.buffer[i] 	 = 0;
		serial.buffer[i+1] = 0;
		serial.buffer[i+2] = 0;
		serial.buffer[i+3] = 0;
		//fill
//		serial.buffer[i] 	 |= data[i/4];
//		serial.buffer[i+1] |= data[i/4] >> 8;
//		serial.buffer[i+2] |= data[i/4] >> 16;
//		serial.buffer[i+3] |= data[i/4] >> 24;
		serial.buffer[i] 	 |= data[i/4] >> 24;
		serial.buffer[i+1] |= data[i/4] >> 16;
		serial.buffer[i+2] |= data[i/4] >> 8;
		serial.buffer[i+3] |= data[i/4];
	}
	
	Serial_send_response(serial.buffer, lenght * 4);
}

uint8_t Serial_ready(void)
{
	uint8_t ret = 0;
	
	/* If received 1 byte and more */
	if (serial.counterRx > 0)
	{
		if ((HAL_GetTick() - serial.timerCounter) > SERIAL_PACKAGE_DELAY) 
		{
			Serial_rec1b_disable();
			
			serial.numberRx 	= serial.counterRx;
			serial.counterRx 	= 0;
			serial.Ready 			= true;
			
			ret = 1;
		}
	}
	return ret;
}
