/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "rtc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LCD/lcd.h"
#include "KEY/key.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
extern uint32_t key_press_time1;
RTC_DateTypeDef GetDate;  //获取日期结构体
RTC_TimeTypeDef GetTime;   //获取时间结构体
uint8_t key1 = 0;
uint8_t key2 = 0;
uint8_t TZ_Flag= 0;
uint8_t HF_Flag = 0;
uint8_t Ajust_Flag = 0;
static uint8_t long_press_triggered = 0;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  GetTime.Hours = 17;
	GetTime.Minutes = 7;
	GetTime.Seconds = 50;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	lcd_init();
	lcd_clear(WHITE);
  key_init();
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RTC_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_RTC_SetTime(&hrtc,&GetTime,RTC_FORMAT_BIN);
//	key = key_scan(1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		 key1 = key_scan1(1);
		 key2 = key_scan2(0);
//		printf("%d\r\n",key);
//		printf("HF:%d\r\n",HF_Flag);
//		printf("%d\r\n",key_press_time);
//		if (HF_Flag == 0) 
//		{
//        key = key_scan1(1);  // 连续按模式
//    } 
//		else 
//		{
//        key = key_scan1(0);  // 不支持连续按模式
//    }

		if (key1 == 1) 
		{
			if (key_press_time1 > 60 && !long_press_triggered) {
//					printf("长按\r\n");
					// 切换 TZ_Flag 状态
					TZ_Flag = !TZ_Flag;

					if (TZ_Flag == 1) 
					{
							__HAL_RCC_RTC_DISABLE(); // 禁用 RTC
							HF_Flag = 1;             // 设置 HF_Flag
					} 
					else 
					{
							if (HF_Flag == 1) 
							{
									__HAL_RCC_RTC_ENABLE(); // 启用 RTC
									HAL_RTC_SetTime(&hrtc, &GetTime, RTC_FORMAT_BIN);
									HF_Flag = 0;
							}
					}

					// 标记长按已触发
					long_press_triggered = 1;
			}
		} 
		else 
		{
			// 松开按键后，重置长按触发标志
			long_press_triggered = 0;
		}
		if(TZ_Flag == 1)
		{
				if(key1 ==1)
				{
					 if(Ajust_Flag <2)
					 {
							Ajust_Flag ++;
					 }
					 else
					 {
							Ajust_Flag =0;
					 }
					 
				}
				if(Ajust_Flag == 0)
				{
					if(key2 == 2 )
					{
						if(GetTime.Hours < 23)
						{							
								GetTime.Hours++;
						}
						else
						{
							  GetTime.Hours = 0;
						}
	//					 HAL_RTC_SetTime(&hrtc,&GetTime,RTC_FORMAT_BIN);
					}
					else if(key2 ==3)
					{
						if(GetTime.Hours > 0)
						{							
								GetTime.Hours--;
						}
						else
						{
							  GetTime.Hours = 23;
						}
	//					HAL_RTC_SetTime(&hrtc,&GetTime,RTC_FORMAT_BIN);
					}	
				}
				else if(Ajust_Flag == 1)
				{
					if(key2 == 2)
					{
						 if(GetTime.Minutes <59)
						 {
								GetTime.Minutes++;
						 }
						 else
						 {
								GetTime.Minutes = 0;
						 }
	//					 HAL_RTC_SetTime(&hrtc,&GetTime,RTC_FORMAT_BIN);
					}
					else if(key2 ==3)
					{
						
						 if(GetTime.Minutes >0)
						 {
								GetTime.Minutes--;
						 }
						 else
						 {
								GetTime.Minutes = 59;
						 }
	//					HAL_RTC_SetTime(&hrtc,&GetTime,RTC_FORMAT_BIN);
					}	
				
				}
				else if(Ajust_Flag == 2)
				{
					if(key2 == 2)
					{
						 if(GetTime.Seconds <59)
						 {
								GetTime.Seconds++;
						 }
						 else
						 {
								GetTime.Seconds = 0;
						 }
	//					 HAL_RTC_SetTime(&hrtc,&GetTime,RTC_FORMAT_BIN);
					}
					else if(key2 ==3)
					{
						 if(GetTime.Seconds >0)
						 {
								GetTime.Seconds--;
						 }
						 else
						 {
								GetTime.Seconds = 59;
						 }
	//					HAL_RTC_SetTime(&hrtc,&GetTime,RTC_FORMAT_BIN);
					}	
				
				}
		}
		
		if (TZ_Flag == 1)
    {
      // Display the updated time on the LCD
			lcd_show_string(20,150,80,20,24,"Adjust",RED);
      lcd_show_num(20, 20, GetTime.Hours, 2, 24, RED);
      lcd_show_num(20, 45, GetTime.Minutes, 2, 24, RED);
      lcd_show_num(20, 70, GetTime.Seconds, 2, 24, RED);
    }
    else
    {
      // Normal RTC mode, retrieve time from RTC and display
      HAL_RTC_GetTime(&hrtc, &GetTime, RTC_FORMAT_BIN);
      HAL_RTC_GetDate(&hrtc, &GetDate, RTC_FORMAT_BIN);
			lcd_show_string(20,150,80,20,24,"       ",RED);
      lcd_show_num(20, 20, GetTime.Hours, 2, 24, RED);
			lcd_show_CN(50,20,6,24,RED);
      lcd_show_num(20, 45, GetTime.Minutes, 2, 24, RED);
			lcd_show_CN(50,45,7,24,RED);
      lcd_show_num(20, 70, GetTime.Seconds, 2, 24, RED);
			lcd_show_CN(50,70,8,24,RED);
    }

    // Debugging information
//    printf("%d\r\n", TZ_Flag);
//    printf("%02d/%02d/%02d\r\n", 2000 + GetDate.Year, GetDate.Month, GetDate.Date);
//    printf("%02d:%02d:%02d\r\n", GetTime.Hours, GetTime.Minutes, GetTime.Seconds);

		  lcd_show_num(20, 200, TZ_Flag, 2, 24, RED);
//    HAL_Delay(1000);

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void delay_ms(uint32_t ms)
{
		uint32_t ms_uwTick = uwTick;
	  while(ms_uwTick - uwTick < ms);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
