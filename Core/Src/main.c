/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_sht20.h"
#include "user_oled.h"
#include "user_app.h"
#include <stdio.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t num_show = 65535;
int32_t unum_show = -10000;
uint8_t char_test = 'L';
extern volatile SHT20_TemRH_Val gTemRH_Val;	//存储温湿度�??
extern int32_t n_sp02; //SPO2 value
extern int32_t n_heart_rate;   //heart rate value
uint8_t show_flag;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#pragma import(__use_no_semihosting) //关闭半主机模�?
//标准库需要的支持函数
struct __FILE
{
	int handle;
};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
x = x;
}
	
//重定义fputc函数
int fputc(int ch,FILE*f)
{
	uint8_t temp[1] = {ch};
	HAL_UART_Transmit (&huart1,temp,1,2);
	return ch;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

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
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */
  OLED_Init();

 
  
  //Update_HeartRateInfo();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	//printf("2025/04/16\n");
	if(show_flag == 0){
		OLED_ShowString(1,1,"/**************/");
		
		OLED_ShowString(2,1,"Tem:");
		OLED_ShowString(3,1," RH:");

		BSP_SHT20_GetData();
		OLED_ShowNum(2,5,(int)gTemRH_Val.Tem,2);
		OLED_ShowString(2,7,".");
		OLED_ShowNum(2,8,(int)(100*gTemRH_Val.Tem) - (int)gTemRH_Val.Tem*100 ,2);
		
		OLED_ShowNum(3,5,(int)gTemRH_Val.Hum,2);
		OLED_ShowString(3,7,".");
		OLED_ShowNum(3,8,(int)(100*gTemRH_Val.Hum) - 100*(int)gTemRH_Val.Hum ,2);
		OLED_ShowString(3,10,"%");
		
		OLED_ShowString(4,1,"/**************/");
	}
	
	if(show_flag  == 1){
		OLED_ShowString(1,1,"Hard & Soft");
		OLED_ShowString(2,1,"Designing by");
		OLED_ShowString(3,1,"LYdong");
		OLED_ShowString(4,1,"2025/04/23 V1.0");
	}
	  
	//Update_HeartRateInfo();
	//OLED_ShowNum(4,4,n_heart_rate/4,2);
	//OLED_ShowNum(4,13,n_sp02,2);

	HAL_Delay (1000);
	  
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
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
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	HAL_Delay (5);//延迟5ms，我害怕会不小心卡死
	if(GPIO_Pin == GPIO_PIN_11){
		if(HAL_GPIO_ReadPin (GPIOA,GPIO_PIN_11) == GPIO_PIN_RESET) {
			show_flag = 1;      //如果是下降沿触发的话说明按下了，显示版本信息
		}
		if(HAL_GPIO_ReadPin (GPIOA,GPIO_PIN_11) == GPIO_PIN_SET) {
			show_flag = 0;      //如果是上升沿触发的话说明松开了，显示测量到的温湿度
		}
	}
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
