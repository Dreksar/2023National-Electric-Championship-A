#include "Smping.h"
#include "adc.h"
#include "arm_math.h"
float V_ref_in;
int   V_ref_init;

uint16_t AD_Buf[5]={0};


void Get_ADC(AD_Detc *AD_detc)
{

		HAL_ADC_Start_DMA(&hadc1,(uint32_t *)AD_Buf,5);
	  
		V_ref_in=1.2f*4095/AD_Buf[4];
		V_ref_init=(int)(V_ref_in*1000);
	  V_ref_in=3+(V_ref_init-3000)*0.001f;
	
		AD_detc->Udetc_f=(AD_Buf[0]/4095.0f*V_ref_in);
		AD_detc->Vdetc_f=(AD_Buf[1]/4095.0f*V_ref_in);
		AD_detc->Wdetc_f=(AD_Buf[2]/4095.0f*V_ref_in);
	  
//	  AD_detc->Uidetc_f=(AD_Buf[3]/4095.0f*3.3f);
//	  AD_detc->Videtc_f=(AD_Buf[4]/4095.0f*3.3f);
//	  AD_detc->Widetc_f=(AD_Buf[5]/4096.0f*3.3f);
	
}

//void Get_Real(AD_Detc *AD_detc)
//{
//	AD_detc->uV_ref=Ave_Sam(AD_detc->Udetc_f,10,&Ave_uV);
//	AD_detc->uI_ref=Ave_Sam(AD_detc->Uidetc_f,10,&Ave_uI);
//	AD_detc->vV_ref=Ave_Sam(AD_detc->Vdetc_f,10,&Ave_vV);
//	AD_detc->vI_ref=Ave_Sam(AD_detc->Videtc_f,10,&Ave_vI);
//	AD_detc->wV_ref=Ave_Sam(AD_detc->Wdetc_f,10,&Ave_wV);
//	AD_detc->wI_ref=Ave_Sam(AD_detc->Widetc_f,10,&Ave_wI);
//	
//	AD_detc->uV_data=(AD_detc->uV_ref-1.654f)/0.0202f;
//	AD_detc->uI_data=(AD_detc->uI_ref-1.654f)/0.4f;
//	AD_detc->vV_data=(AD_detc->vV_ref-1.654f)/0.0202f;
//	AD_detc->vI_data=(AD_detc->vI_ref-1.654f)/0.4f;
//	AD_detc->wV_data=(AD_detc->wV_ref-1.654f)/0.0202f;
//	AD_detc->wI_data=(AD_detc->wI_ref-1.654f)/0.4f;
//	
//}
float Valid_vuale(float vuale,int time)
{
	float ret=0;
	static uint16_t time_count=0;
	static uint32_t sum=0;
	time_count++;
	sum+=vuale*vuale;
	if(time_count==time)
	{
		time_count=0;
		arm_sqrt_f32(sum*1.f/time,&ret);
		sum=0.f;
		return ret;
	}
	
}
//		time_count++;
//		sumU+=U_data*U_data;		
//		sumV+=V_data*V_data;	
//		if(time_count==400)
//			{		
//				time_count=0;				
//				Un=sqrt(sumU*0.0025f);//
//				sumU=0.f;//
//				Vn=sqrt(sumV*0.0025f);//
//				sumV=0.f;//

//			}