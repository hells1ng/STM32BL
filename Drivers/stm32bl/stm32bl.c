#include "stm32bl.h"
#include "serial.h"
#include "parser.h"

STM32BL flash;

/* Use this func it your application for enter to bootloader app */
void STM32BL_go_to_bl(void)
{
		__disable_irq();
		STM32BL_backup_write_reg(STM32BL_BACKUP_MAIN_REG, STM32BL_BACKUP_REG_VAL);
		HAL_NVIC_SystemReset();
}

void STM32BL_flash_init(void)
{
	flash.Address = 0;
	flash.Error = 0;
	flash.PAGEError = 0;
	flash.firstflag = 0;
	flash.starttimer_counter = 0;
	for (uint16_t i = 0; i < STM32BL_BUFF_SIZE; i++)
		flash.buffer[i] = 0x00000000;
}

void STM32BL_reset_and_go_to_app(void)
{
		__disable_irq();
		STM32BL_backup_write_reg(STM32BL_BACKUP_MAIN_REG, STM32BL_BACKUP_REG_VAL_DEF);
		HAL_NVIC_SystemReset();
}

void STM32BL_go_to_app(void)
{
	typedef  void (*pFunction)(void);	
	pFunction Jump_To_Application;
	uint32_t JumpAddress;

	uint32_t startAddress = STM32BL_FLASH_USER_START_ADDR + STM32BL_FLASH_USER_START_ADDR_OFFSET;
	
	SCB->VTOR = NVIC_VectTab_FLASH | ((startAddress) & (uint32_t)0x1FFFFF80);
	
	JumpAddress = *(__IO uint32_t*) (startAddress + STM32BL_WORD_SIZE);
	Jump_To_Application = (pFunction) JumpAddress;
	__set_MSP(*(__IO uint32_t*) (startAddress));		
	Jump_To_Application(); 
}


void STM32BL_check_starttimer(void)
{
#if defined(STM32BL_ENABLE_START_DELAY)

	/* this condition will be TRUE only 1 time when power ON */
	if (flash.StartFlag && (HAL_GetTick() - flash.starttimer_counter) > STM32BL_START_TIMER_DELAY ) 
	{					
		__disable_irq();
					
		STM32BL_backup_write_reg(STM32BL_BACKUP_BL_TIMEOUT_REG, STM32BL_BACKUP_REG_VAL);
		HAL_NVIC_SystemReset();
	}
#endif

}


void STM32BL_backup_enable()
{
//	HAL_PWR_EnableBkUpAccess();
	__HAL_RCC_PWR_CLK_ENABLE();
  __HAL_RCC_BKP_CLK_ENABLE();
}

uint32_t STM32BL_backup_read_reg(uint32_t BackupRegister)
{
    RTC_HandleTypeDef RtcHandle;
    RtcHandle.Instance = RTC;
    return HAL_RTCEx_BKUPRead(&RtcHandle, BackupRegister);
}

void STM32BL_backup_write_reg(uint32_t BackupRegister, uint32_t Data)
{
    RTC_HandleTypeDef RtcHandle;
    RtcHandle.Instance = RTC;
    HAL_PWR_EnableBkUpAccess();
    HAL_RTCEx_BKUPWrite(&RtcHandle, BackupRegister, Data);
    HAL_PWR_DisableBkUpAccess();
}

uint8_t STM32BL_Flash_CheckCRC()
{	
	uint32_t Addr = STM32BL_FLASH_CRC_ADDR;

  if ((*(__IO uint32_t*) Addr) == STM32BL_FLASH_CRC_VAL)
		return 1;
	else
		return 0;
}

void STM32BL_check_start_backup(void)
{
#if defined(STM32BL_ENABLE_START_DELAY)
	if (STM32BL_backup_read_reg(STM32BL_BACKUP_BL_TIMEOUT_REG) == STM32BL_BACKUP_REG_VAL_DEF) {
		flash.StartFlag = true; //this flag must set to TRUE only 1 time when power ON
		flash.starttimer_counter = HAL_GetTick();
	}
	else if (STM32BL_backup_read_reg(STM32BL_BACKUP_MAIN_REG) == STM32BL_BACKUP_REG_VAL_DEF && STM32BL_Flash_CheckCRC()) {
		STM32BL_go_to_app();
	};
#else	
	if (STM32BL_backup_read_reg(STM32BL_BACKUP_MAIN_REG) == STM32BL_BACKUP_REG_VAL_DEF && STM32BL_Flash_CheckCRC()) {
		STM32BL_go_to_app();
	};
#endif	
}

void STM32BL_Flash_Read(uint8_t numOfPage, uint8_t numOfWords)
{
		uint32_t Addr = ADDR_FLASH_PAGE_0 + numOfPage * STM32BL_FLASH_PAGE_SIZE;
		uint32_t Addr_end = Addr + numOfWords * STM32BL_WORD_SIZE;
		uint8_t i = 0;
	
		//Fill flashBuffer from flash-memory
    while (Addr < Addr_end)
    {
			flash.buffer[i] = (*(__IO uint32_t*) Addr);
			i++;
      Addr += 4;
    }
}

void STM32BL_Flash_Clean_Buffer(uint8_t words)
{
	for (uint8_t i = 0; i < words; i++)
	{
		flash.buffer[i] = 0x00000000;
	}
}

uint32_t STM32BL_Flash_Erase(void)
{
	static FLASH_EraseInitTypeDef EraseInitStruct;

	HAL_FLASH_Unlock();

	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = STM32BL_FLASH_USER_START_ADDR - 1 * STM32BL_FLASH_PAGE_SIZE;//TODO test 
	EraseInitStruct.NbPages     = 
		(STM32BL_FLASH_USER_END_ADDR - (STM32BL_FLASH_USER_START_ADDR - 1 * FLASH_PAGE_SIZE)) / STM32BL_FLASH_PAGE_SIZE;

	if (HAL_FLASHEx_Erase(&EraseInitStruct, &flash.PAGEError) != HAL_OK)
	{
		flash.Error = HAL_FLASH_GetError();
	}
	
	HAL_FLASH_Lock();
	
	flash.Address = STM32BL_FLASH_USER_START_ADDR + STM32BL_FLASH_USER_START_ADDR_OFFSET;
	
	return flash.Error;	
}

uint8_t STM32BL_Flash_Write(uint8_t * msg, uint16_t lenght, uint8_t offset)
{					
	
	for (uint16_t i = 0; i < lenght; i = i + STM32BL_WORD_SIZE)
	{
		flash.buffer[i/STM32BL_WORD_SIZE] |= msg[i + 3 + offset] << 24;
		flash.buffer[i/STM32BL_WORD_SIZE] |= msg[i + 2 + offset] << 16;
		flash.buffer[i/STM32BL_WORD_SIZE] |= msg[i + 1 + offset] << 8;
		flash.buffer[i/STM32BL_WORD_SIZE] |= msg[i + 0 + offset];
	}
	
	HAL_FLASH_Unlock();	
		
	uint16_t k = 0;
	uint32_t Address_end = flash.Address + lenght;
	
	while (flash.Address < Address_end )
	{
		uint32_t Data = flash.buffer[k];
		
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flash.Address, Data) == HAL_OK)
		{
			flash.Address = flash.Address + 4;
			flash.buffer[k] = 0x00000000;
			k++;
		}
		else break;
	}
	
	HAL_FLASH_Lock();
	
	if (flash.Address >= Address_end )
		return STM32BL_FLASH_RES_OK;
	else 
		return STM32BL_FLASH_RES_ERROR;
}

uint32_t STM32BL_Flash_ErasePage(uint32_t addr_of_page)
{
	static FLASH_EraseInitTypeDef EraseInitStruct;

	HAL_FLASH_Unlock();

	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = addr_of_page;
	EraseInitStruct.NbPages     = 1;

	if (HAL_FLASHEx_Erase(&EraseInitStruct, &flash.PAGEError) != HAL_OK)
	{
		flash.Error = HAL_FLASH_GetError();
	}
	
	HAL_FLASH_Lock();
		
	return flash.Error;	
}

/* Write 32bit Reg in Flash */
uint8_t STM32BL_Flash_Write32(uint32_t addr, uint32_t value)
{	
	HAL_FLASH_Unlock();	
	
	if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, value) == HAL_OK)
	{
		HAL_FLASH_Lock();
		return STM32BL_FLASH_RES_OK;
	}
	else 
	{
		HAL_FLASH_Lock();
		return STM32BL_FLASH_RES_ERROR;
	}
}
uint32_t STM32BL_Flash_Read32(uint32_t addr)
{
	return (*(__IO uint32_t*) addr);
}

