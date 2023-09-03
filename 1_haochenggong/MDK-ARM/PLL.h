#ifndef _PLL_H
#define _PLL_H

#include "main.h"
#include "Cleck.h"
#include "Park.h"

/* À¯œ‡ª∑PIøÿ÷∆    */
#define PLL_MAX_OUT     2000
#define PLL_MIN_OUT    -2000





typedef struct 
{
    /* input data */
   float Ref;
	 float Fed;
	 float Err;
	float Err_Pre;
	float Kp;
	float Ki;
	float Loopout;
	float MaxLoopout;
	float MinLoopout;
	float Temp;
	
}PLLPID;



void PLL_Init(CLECK *Cl,PARK *Pa);
void ThreePhasePLL(CLECK *Cl,PARK *Pa,uint32_t Ts,float a,float b,float c);
#endif