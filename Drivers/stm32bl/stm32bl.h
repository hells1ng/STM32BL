
#ifndef __stm32bl_H
#define __stm32bl_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32bl_conf.h"
#include <stdbool.h>

#define STM32BL_CMD_REQUEST		    0x00
#define STM32BL_CMD_NO_CMD				0xff
#define STM32BL_CMD_RESTART				0x80
#define STM32BL_CMD_ERASE 				0x32
#define STM32BL_CMD_FLASH	 				0x34
#define STM32BL_CMD_GOTOAPP				0x35
#define STM32BL_CMD_READDATA			0x36
#define STM32BL_CMD_FLASHCRC			0x37
#define STM32BL_CMD_WRITE_1REG		0x38
#define STM32BL_FLASH_RES_OK			0x00
#define STM32BL_FLASH_RES_ERROR		0x01

#define NVIC_VectTab_FLASH        ((uint32_t)0x08000000)

typedef struct {
	uint8_t  	firstflag;
	uint32_t 	starttimer_counter;
	uint32_t 	buffer[STM32BL_BUFF_SIZE];
	uint32_t 	Address, PAGEError , Error ;
	bool 			StartFlag;
} STM32BL;

/* functions with backup regs */
void 			STM32BL_backup_enable(void);
uint32_t 	STM32BL_backup_read_reg(uint32_t BackupRegister);
void 			STM32BL_backup_write_reg(uint32_t BackupRegister, uint32_t Data);
void 			STM32BL_check_start_backup(void);

/* jump functions */
void 			STM32BL_go_to_bl(void);
void 			STM32BL_reset_and_go_to_app(void);
void 			STM32BL_go_to_app(void);	


/* functions for read-write flash */
void 			STM32BL_flash_init(void);
void 			STM32BL_Flash_Clean_Buffer(uint8_t words);
void 			STM32BL_Flash_Read(uint8_t numOfPage, uint8_t numOfWords);
uint32_t 	STM32BL_Flash_Erase(void);
uint8_t 	STM32BL_Flash_Write(uint8_t * msg, uint16_t lenght, uint8_t offset);
uint8_t 	STM32BL_Flash_CheckCRC(void);

uint32_t 	STM32BL_Flash_ErasePage(uint32_t addr_of_page); /* erase 1 page */
uint8_t 	STM32BL_Flash_Write32(uint32_t addr, uint32_t value); /* write 1 32bit Reg */
uint32_t 	STM32BL_Flash_Read32(uint32_t addr); /* read 1 32bit Reg */

void 			STM32BL_check_starttimer(void);

#ifdef __cplusplus
}
#endif
#endif 


