#ifndef _SPWM_PID_H
#define _SPWM_PID_H

#include "SPWM_pid.h"
#include "main.h"

typedef struct
{
	float Err_Pre;
	float Kp;
	float Ki;
	 float out;
	 float temp;
	 float error;
	float maxout;
	float minout;
	float error_sum;
	
}PIDV;
void PID_init(PIDV *pid);
void Spwm_pid(PIDV *pid,float target,float Vin);
#endif


