
#include "parser.h"
#include "stm32bl.h"
 
uint8_t 	RS485_ADDR = 0;
 
uint8_t Parser_check_request(uint8_t * msg, uint16_t len)
{
	uint8_t ret = STM32BL_CMD_NO_CMD;
	uint8_t crc = START_BYTE;
	
	for (uint16_t i = 1; i < len - 1; i++)//не проверяем 1 стартбайт
	{
		if (i < CMD_NUMB_START_BYTES)
		{ 
			if (msg[i] != START_BYTE) return ret;
		}
		if (i == CMD_NUMB_START_BYTES) 
		{
			if (msg[i] != RS485_ADDR)
				if (msg[i] != RS485_ADDR_DEF)
					return ret;
		}
		crc ^= msg[i];
	}
	if (crc != msg[len - 1]) 
		return ret;
	
	return msg[CMD_NUMB_START_BYTES + 1]; //FlagByte
}

/*	Получаем из посылки число байт флеша, ожидаемых в следующей посылке
*/
uint16_t Parser_GetData16(uint8_t * msg)
{
	return (msg[CMD_NUMB_START_BYTES + 2] << 8 | msg[CMD_NUMB_START_BYTES + 3]);
}



