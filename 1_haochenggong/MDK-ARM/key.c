#include "key.h"
#include "main.h"
#include "stm32f4xx.h"                  // Device header

int lianxu=1;
int first=1;
	//KEY0>KEY1>KEY2>KEY3
uint8_t KEY_Scan(uint8_t mode)
{	 
		static uint8_t key_up=1;//按键松开标志
		if(mode)
			key_up=1;  //支持连按  
		if(key_up&&(KEY0==0||KEY1==0))
		{
		  if(first==0&&lianxu==0)
			{
				Delay_ms(500);	
				lianxu=1;
			}
			Delay_ms(10);
//			HAL_Delay(10);
			key_up=0;
			first=0;
			if(KEY0==0)return KEY0_PRES;
			else if(KEY1==0)return KEY1_PRES;
		}
		else if(KEY0==1&&KEY1==1)
		{
			key_up=1; 
			first=1;
			lianxu=0;
		}

		return 0;			//没有按键按下
}

uint8_t KEY_Scan1(uint8_t mode)
{	 
		static uint8_t key_up=1;//按键松开标志
		if(mode)
			key_up=1;  //支持连按  
		if(key_up&&(KEY2==0||KEY3==0||KEY4==0||KEY5==0))
		{
			Delay_ms(10);
//			HAL_Delay(10);
			key_up=0;
			if(KEY2==0)return KEY2_PRES;
			else if(KEY3==0)return KEY3_PRES;
			else if(KEY4==0)return KEY4_PRES;
			else if(KEY5==0)return KEY5_PRES;
		}
		else if(KEY2==1&&KEY3==1&&KEY4==1&&KEY5==1)
		{
			key_up=1; 
		}

		return 0;			//没有按键按下
}

/**
  * @brief  ?????
  * @param  xus ????,??:0~233015
  * @retval ?
  */
void Delay_us(uint32_t xus)
{
	SysTick->LOAD = 72 * xus;				//????????
	SysTick->VAL = 0x00;					//???????
	SysTick->CTRL = 0x00000005;				//??????HCLK,?????
	while(!(SysTick->CTRL & 0x00010000));	//?????0
	SysTick->CTRL = 0x00000004;				//?????
}

/**
  * @brief  ?????
  * @param  xms ????,??:0~4294967295
  * @retval ?
  */
void Delay_ms(uint32_t xms)
{
	while(xms--)
	{
		Delay_us(1000);
	}
}

