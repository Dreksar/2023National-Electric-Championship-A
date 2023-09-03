#ifndef __KEY_H
#define __KEY_H

#include "main.h"

#define KEY0  HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12)  
#define KEY1  HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8)  
#define KEY2  HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11) 
#define KEY3  HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_9)
#define KEY4  HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)
#define KEY5  HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)

 
#define KEY0_PRES 	 1	//KEY0
#define KEY1_PRES   2	//KEY1
#define KEY2_PRES   3	
#define KEY3_PRES   4	
#define KEY4_PRES   5	
#define KEY5_PRES   6	

 
uint8_t KEY_Scan(uint8_t mode); 	
uint8_t KEY_Scan1(uint8_t mode);
void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);
#endif
