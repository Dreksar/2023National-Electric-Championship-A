#ifndef _SMPING_H
#define _SMPING_H


typedef struct
{
	//·´Cleck±ä»»
	float Udetc_f;
	float Vdetc_f;
	float Wdetc_f;
	float Uidetc_f;
	float Videtc_f;
	float Widetc_f;
	float uV_ref;
	float uI_ref;
	float wV_ref;
	float wI_ref;
	float vV_ref;
	float vI_ref;
	float uV_data;
	float uI_data;
	float wV_data;
	float wI_data;
	float vV_data;
	float vI_data;
	float uV_valid;
	float uI_valid;
	float vV_valid;
	float vI_valid;
	
}AD_Detc;

void Get_ADC(AD_Detc *AD_detc);
void Get_Real(AD_Detc *AD_detc);
float Valid_vuale(float vuale,int time);

#endif