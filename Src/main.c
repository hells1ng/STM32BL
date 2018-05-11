
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "serial.h"
#include "stm32bl.h"
#include "parser.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
extern 	Serial 		serial; 		/* from serial.h */
extern  STM32BL 	flash;			/* from stm32bl.h */
extern 	uint8_t 	RS485_ADDR;	/* from parser.h */

uint8_t txOK[] = "OK";
uint8_t txERROR[] = "ERROR";

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

void send_OK()
{
	Serial_send_response(txOK, COUNTOF(txOK) - 1);
}
void send_ERROR()
{
	Serial_send_response(txERROR, COUNTOF(txERROR) - 1);
}

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	
	STM32BL_backup_enable();			/* enable use backup CPU regs */

	STM32BL_check_start_backup();	/* check start conditions */
	
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

	RS485_ADDR = (uint8_t)STM32BL_Flash_Read32(STM32BL_FLASH_SETTINGS_ADDR);
	
	Serial_init();
	STM32BL_flash_init();
	
	Serial_ready_to_receive();
	
	if (flash.StartFlag == false) // if false - it's not first entry, so send OK
		send_OK();
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		if (serial.Ready)
		{
			serial.Ready = false;
			
			switch (Parser_check_request(serial.buffer, serial.numberRx)) {
				
				case STM32BL_CMD_RESTART:
					
						__disable_irq();
						STM32BL_backup_write_reg(STM32BL_BACKUP_MAIN_REG, STM32BL_BACKUP_REG_VAL);
						HAL_NVIC_SystemReset();
						break;
				
				case STM32BL_CMD_ERASE: 
					
						STM32BL_Flash_Erase();
						Serial_ready_to_receive();
						send_OK();
						break;
				
				case STM32BL_CMD_FLASH:
					
						if (STM32BL_Flash_Write(serial.buffer, 
																		serial.numberRx - CMD_NUBM_PROTOCOL_BYTES, 
																		CMD_NUMB_START_BYTES + 2) == STM32BL_FLASH_RES_OK)
						{
							Serial_ready_to_receive();
							send_OK();
						}
						else 
						{
							Serial_ready_to_receive();
							send_ERROR();
						}
						break;
						
				case STM32BL_CMD_FLASHCRC:
					
						if (STM32BL_Flash_Write32(STM32BL_FLASH_CRC_ADDR, STM32BL_FLASH_CRC_VAL) == STM32BL_FLASH_RES_OK)
						{
							Serial_ready_to_receive();
							HAL_Delay(5);
							send_OK();
						}
						else 
						{
							Serial_ready_to_receive();
							HAL_Delay(5);
							send_ERROR();
						}
						break;
						
				case STM32BL_CMD_READDATA:
				{
					/* read num of Words in Page */
//						uint8_t Page 	= Parser_GetData16(serial.buffer) >> 8;
//						uint8_t Words = Parser_GetData16(serial.buffer);
//				
//						Serial_ready_to_receive();
//						STM32BL_Flash_Read(Page, Words);//page and words
//						HAL_Delay(1);
//						Serial_send_response32(flash.buffer, Words);
					
					/* now in READDATA read only 1 reg settings - RS485 addr*/
						Serial_ready_to_receive();
						flash.buffer[0] = STM32BL_Flash_Read32(STM32BL_FLASH_SETTINGS_ADDR);//page and words
						HAL_Delay(5);
						Serial_send_response32(flash.buffer, 1);
						break;
				}						
				case STM32BL_CMD_WRITE_1REG: //only rs485_addr
				{
						uint8_t rs485_new_addr = Parser_GetData16(serial.buffer);
				
						STM32BL_Flash_ErasePage(STM32BL_FLASH_SETTINGS_ADDR);
						if (STM32BL_Flash_Write32(STM32BL_FLASH_SETTINGS_ADDR, rs485_new_addr) == STM32BL_FLASH_RES_OK)
						{
							/*update new address*/
							RS485_ADDR = (uint8_t)STM32BL_Flash_Read32(STM32BL_FLASH_SETTINGS_ADDR);
							Serial_ready_to_receive();
							send_OK();
						}
						else 
						{
							Serial_ready_to_receive();
							send_ERROR();
						}
						break;
				}
				case STM32BL_CMD_GOTOAPP:
					
						HAL_Delay(5);
						if (STM32BL_Flash_CheckCRC())
						{
							send_OK();
							STM32BL_reset_and_go_to_app();
						}
						else 
						{
							Serial_ready_to_receive();
							send_ERROR();
						}
						break;
				
				default:
						Serial_ready_to_receive();
						break;
				
			} //end of switch
		}
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
void HAL_SYSTICK_Callback(void)
{
	Serial_ready();
	STM32BL_check_starttimer();
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/