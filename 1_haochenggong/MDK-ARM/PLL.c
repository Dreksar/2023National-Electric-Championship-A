#include "PLL.h"
PLLPID  PLL_pid;
#include "math.h"
#define PLL_KP          2.4f*2.f //2.4f
#define PLL_KI          0.02f * 2.0f//0.04    // 0.001

void PLL_Init(CLECK *Cl,PARK *Pa)   
{
	Pa->Vd=0.f;
	Pa->Vq=0.f;
	Pa->theta=PI/2.f;//100.f*PI
	Pa->SinWt=0.f;
	Pa->CosWt=0.f;
	
	Cl->Valpha=0.f;
	Cl->Vbeta=0.f;
	
	PLL_pid.Fed=0.f;
	PLL_pid.Ref=0.f;
	PLL_pid.Err=0.f;
	PLL_pid.Err_Pre=0.f;
	PLL_pid.Ki=PLL_KI ;
  PLL_pid.Kp=PLL_KP;
	PLL_pid.Loopout=0 ;
	PLL_pid.MaxLoopout=PLL_MAX_OUT ;
	PLL_pid.MinLoopout=PLL_MIN_OUT ;
	PLL_pid.Temp=0.f;

}
void ThreePhasePLL(CLECK *Cl,PARK *Pa,uint32_t Ts,float a,float b,float c)
{
	
	Cl->Valpha=(a-0.5f*b-0.5f*c)*2.f/3.f;
  Cl->Vbeta=(0.8660254f*b-0.8660254f*c)*2.f/3.f;
	
	Pa->Vd=Pa->CosWt*Cl->Valpha+Pa->SinWt*Cl->Valpha;
	Pa->Vq=Pa->SinWt*Cl->Valpha+Pa->CosWt*Cl->Valpha;
//	Pa->Vd=3;
//	Pa->Vq=2;
	PLL_pid.Fed=0;
	PLL_pid.Ref=Pa->Vq;
	PLL_pid.Err=PLL_pid.Ref-PLL_pid.Fed;
	
	PLL_pid.Temp=PLL_pid.Ki*PLL_pid.Err;
	PLL_pid.Temp+=PLL_pid.Kp*(PLL_pid.Err-PLL_pid.Err_Pre);
	PLL_pid.Loopout+=PLL_pid.Temp;
	PLL_pid.Err_Pre=PLL_pid.Err;
	
	if(PLL_pid.Loopout>PLL_pid.MaxLoopout)
	{
		PLL_pid.Loopout=PLL_pid.MaxLoopout;
	}
		if(PLL_pid.Loopout<PLL_pid.MinLoopout)
	{
		PLL_pid.Loopout=PLL_pid.MinLoopout;
	}
//	if(PLL_pid.Loopout<=0)
//	{
//		PLL_pid.Loopout=-PLL_pid.Loopout;
//	}
	Pa->theta+=(PLL_pid.Loopout/Ts);
	
	if(Pa->theta>2*PI)
	{
		Pa->theta=Pa->theta-2*PI;
	}
		if(Pa->theta<0)
	{
		Pa->theta=Pa->theta+2*PI;
	}
	Pa->CosWt=cosf(Pa->theta);
	Pa->SinWt=sinf(Pa->theta);
}