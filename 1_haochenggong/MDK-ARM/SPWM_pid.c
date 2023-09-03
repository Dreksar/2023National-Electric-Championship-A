#include "SPWM_pid.h"


void PID_init(PIDV *pid)
{
	pid->Kp=0.00007f;
	pid->Ki=0.0f;
	pid->error=0;
	pid->maxout=1.f;
	pid->minout=0.2f;
	
}
void Spwm_pid(PIDV *pid,float target,float Vin)
{
	pid->error=target - Vin;
  pid->error_sum +=  pid->error;
	pid->temp=pid->Kp*pid->error+pid->Ki*pid->error_sum;
	pid->out+=pid->temp;
	if(pid->out>pid->maxout)
	{
		pid->out=pid->maxout;
	}		
	if(pid->out<pid->minout)
	{
		pid->out=pid->minout;
	}

}