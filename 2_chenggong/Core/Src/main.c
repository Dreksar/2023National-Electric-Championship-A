/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cleck.h"
#include "Svpwm.h"
#include "Ave_Sam.h"
#include "Park.h"
#include "math.h"
#include "PLL.h"
#include "arm_math.h"
#include "SOGI_PLL.h"
#include "SPWM_pid.h"
#include "vofa.h"
#include "Smping.h"
#include "oled.h"
#include "key.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define iq_tar     0


//#define kp_q       -8.2f  //-5.f                并网逆变10v 1A分配kp_q=-9.f  ki_q=-0.08f
//#define ki_q       -0.08f   //-20.0f
//#define kp_d       -13.f  //-12.5f                                             //-12.5f
//#define ki_d       -0.6f  //-5.f             2号

//#define kp_q       -10.f  //-5.f                并网逆变10v 1A分配kp_q=-9.f  ki_q=-0.08f
//#define ki_q       -0.010f   //-20.0f
//#define kp_d       -10.f  //-12.5f                                             //-12.5f
//#define ki_d       -0.1f  //-5.f    


float  kp_q    =   -0.f;  //-5.f                并网逆变10v 1A分配kp_q=-9.f  ki_q=-0.08f
float  ki_q    =   -0.f ;  //-20.0f
float  kp_d    =   -0.f  ;//-12.5f                                             //-12.5f
float ki_d     =  -0.f ; //-5.f                                                 //-5.f
CLECK Cleck,Cleck_1;
PARK  Park;
AD_Detc AD_buf;
extern PLL p;
extern PR PRII;
PIDV pid;
Fil_TypeDef fil_i={0.f,0.f,0.f,0.f,0.f,0.f,},fil_u={0.f,0.f,0.f,0.f,0.f,0.f,};
float U_spwm,V_spwm,W_spwm,sumU;
float Sin_A,Sin_B,Sin_C;

float Un=1.f,Vn=1.f,Wn=1.f;

float Out;
float Tim_1_channe1_Compare,Tim_1_channe2_Compare;

float iq_error,iq_error_sum;
float id_error,id_error_sum;
float uab,uab_d,uab_q;
float target_nibian=22.f;
uint16_t time_count;
uint16_t time_start;
uint8_t key;
uint8_t key1;
float Iset;
float IoutT;
float id_tar  =   -1.414f;
float x=0;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_DAC_Init();
  MX_USART1_UART_Init();
  MX_TIM4_Init();
  MX_I2C1_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
	OLED_Init();
	OLED_CLS();
	OLED_ShowStr(39,2,"STOP   ",2);
	OLED_ShowStr(0,4,"Iset:",2);
	OLED_ShowStr(68,4,".",2);
	OLED_ShowStr(98,4,"A",2);
	OLED_ON();
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_Base_Start_IT(&htim4);
	HAL_TIM_Base_Start_IT(&htim6);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
	PLL_Init(&Cleck,&Park);
	HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
	PID_init(&pid);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		key=KEY_Scan(1);
		switch(key)
		{
			case 1:  Iset+=0.01;
			break;
			case 2:  Iset-=0.01;
			break;
		}
		if(Iset>2)
			Iset=2;
		if(Iset<1)
			Iset=1;
		
		key1=KEY_Scan1(0);
		switch(key1)
		{
			case 3:  id_tar=Iset*-1.414f;
			break;
			case 4:  
				 kp_q     =  -4.0f ; //-5.f                并网逆变10v 1A分配kp_q=-9.f  ki_q=-0.08f     -6.5f
         ki_q     =  -0.2f;   //-20.0f                                                      -0.15f
         kp_d     =  -4.0f ; //-12.5f                                             //-12.5f   -6.5f
         ki_d     =  -0.2f ; //-5                                                           -0.15f
			   x=0.8f;
			   id_tar  =   -1.414f*2.0f;
								OLED_ShowStr(39,2,"PARALLE",2);
			break;
			case 5:
			   kp_q     =  -7.4f ; //-5.f                并网逆变10v 1A分配kp_q=-9.f  ki_q=-0.08f
         ki_q     =  -0.3f;   //-20.0f
         kp_d     =  -7.4f ; //-12.5f                                             //-12.5f
         ki_d     =  -0.3f ;
			  x=0.812f;
			
								OLED_ShowStr(39,2,"ON-GRID",2);
			break;
			case 6:  
				kp_q     =  -0.f ; //-5.f                并网逆变10v 1A分配kp_q=-9.f  ki_q=-0.08f
         ki_q     =  -0.f;   //-20.0f
         kp_d     =  -0.f ; //-12.5f                                             //-12.5f
         ki_d     =  -0.0f ;
								OLED_ShowStr(39,2,"STOP   ",2);
			break;
		}
	  uint8_t 	Itemp[4];
		if(Iset>1)
			IoutT=(Iset*0.4602+0.5324)*100;
		else
			IoutT=Iset*100;
		Itemp[0] = (uint8_t)(IoutT/1000);
		Itemp[1] = (uint8_t)((IoutT-(uint8_t)Itemp[0]*1000)/100);
		Itemp[2] = (uint8_t)((IoutT-(uint16_t)Itemp[0]*1000-(uint16_t)Itemp[1]*100)/10);
		Itemp[3] = (uint8_t)(IoutT-(uint16_t)Itemp[0]*1000-(uint16_t)Itemp[1]*100-(uint16_t)Itemp[2]*10);
		OLEDShowData(50,4,Itemp[0]);
		OLEDShowData(60,4,Itemp[1]);
		OLEDShowData(75,4,Itemp[2]);
		OLEDShowData(85,4,Itemp[3]);
			OLED_ON();	

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim==&htim3)
	{
		Sin_Start(10000,1,sqrt_3);
    Get_ADC(&AD_buf);
		
		AD_buf.uV_ref=Ave_Sam_u(AD_buf.Udetc_f,10);
		AD_buf.uI_ref=Ave_Sam_i(AD_buf.Vdetc_f,10);
		AD_buf.wV_ref=Ave_Sam_w(AD_buf.Wdetc_f,10);
		///////////////////对AD_Buf取平均
		AD_buf.uV_data=(AD_buf.uV_ref-1.64f)/0.0101f;
		AD_buf.wV_data=(AD_buf.wV_ref-1.64f)/0.0101f;
    AD_buf.uI_data=(AD_buf.uI_ref-1.654f)/0.4f;
//		send_data_to_vofa4(AD_buf.uV_data,Un,AD_buf.wV_data,fil_i.d);
//		time_count++;
//		sumU+=AD_buf.uV_data*AD_buf.uV_data;
//		if(time_count==400)
//			{		
//				time_count=0;				
//				Un=sqrt(sumU*0.0025f);//
//				sumU=0.f;//
//			}
	}

	if(htim==&htim2)
	{
		Sogi_pll(-AD_buf.uV_data,&fil_u);

		SOGI_FIL(AD_buf.uI_data,&fil_i);


	
	}
	if(htim==&htim6)
	{
				iq_error=iq_tar - fil_i.q;
		iq_error_sum+=iq_error*0.00005f;

						
		id_error=id_tar - fil_i.d;
		id_error_sum+=id_error*0.00005f;
	    if(iq_error_sum>0.8f)
		{
			iq_error_sum=0.8f;
		}
		if(iq_error_sum<-0.8f)
		{
			iq_error_sum=-0.8f;
		}
		if(id_error_sum>0.8f)
		{
			id_error_sum=0.8f;
		}
		if(id_error_sum<-0.8f)
		{
			id_error_sum=-0.8f;
		}
		uab_d=fil_u.d + (kp_d*id_error + ki_d*id_error_sum) - 100*PI*0.002f*fil_i.q ;
		uab_q= -(kp_q*iq_error + ki_q*iq_error_sum) + 100*PI*0.002f*fil_i.d;
		uab=(uab_d*arm_cos_f32(p.angle* PI) + uab_q*arm_sin_f32(p.angle* PI));
		if(AD_buf.wV_data<=1.0f)
		{
			AD_buf.wV_data=1.0f;
		}
		Out=uab/AD_buf.wV_data/1.f;
		if(Out>1)Out=1;
		if(Out<-1)Out=-1;
		Tim_1_channe2_Compare=(8200*Out*0.5f + 8200*0.5f)*0.5f*x;
		Tim_1_channe1_Compare=(-8200*Out*0.5f + 8200*0.5f)*0.5f*x;

		 __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,(unsigned int)Tim_1_channe1_Compare);
	   __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_3,(unsigned int)Tim_1_channe2_Compare);
	}
	
	
	
//	if(htim==&htim4)
//	{
//		  time_count++;
//		  sumU+=AD_buf.uV_data*AD_buf.uV_data;
//		if(time_count==400)
//			{		
//				time_count=0;				
//				Un=sqrt(sumU*0.0025f);//
//				sumU=0.f;//
//			}
//     Spwm_pid(&pid,target_nibian,Un);
//		 Tim_1_channe1_Compare=(pid.out*Sin_A*0.5*8400 + 4200)*0.5f;
//	   Tim_1_channe2_Compare=(pid.out*(-Sin_A)*0.5*8400 +4200)*0.5f;
//	   __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,(unsigned int)Tim_1_channe1_Compare);
//	   __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_3,(unsigned int)Tim_1_channe2_Compare);
//	}
	

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
