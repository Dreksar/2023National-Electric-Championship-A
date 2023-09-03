#include "Svpwm.h"
#include "arm_math.h"
#include "math.h"

uint8_t bSector;
float Time;
float a;
extern float Sin_A,Sin_B,Sin_C;
float  wX,wY,wZ;
float  wA,wB,wC;
uint8_t N;
uint8_t N2;
extern float U_spwm,V_spwm,W_spwm,A_spwm,B_spwm,C_spwm;

float Theta;
void Sin_Start(uint16_t Frquence,float k,float Vdc)//k所需要的幅值  Vdc输入直流电压
{
	float Out;
	Out=sqrt_3*k/Vdc;
	a=Frquence/50;
	a=360/a;
			Sin_A=Out*arm_sin_f32(Time/360.f*2*PI);
			Sin_B=Out*arm_sin_f32(Time/360.f*2*PI+2*PI/3);
			Sin_C=Out*arm_sin_f32(Time/360.f*2*PI-2*PI/3);
		Time+=a;
			if(Time>=360.f)
			{
				Time=0;
			}
	
}
void Svpwm(CLECK *Cleck,float a,float b,float c)
{
//		
//Cleck->Valpha=(Sin_A-0.5f*Sin_B-0.5f*Sin_C)*2/3;
//Cleck->Vbeta=(0.8660254f*Sin_B-0.8660254f*Sin_C)*2/3;
//	Cleck->Valpha=(a-0.5f*b-0.5f*c)*2/3;//0.35
//	Cleck->Vbeta=(0.8660254f*b-0.8660254f*c)*2/3;
	
				wX=Cleck->Vbeta*ts;
				wZ=(1.73205080f*Cleck->Valpha-Cleck->Vbeta)/2*ts;//u3
				wY=-(1.73205080f*Cleck->Valpha+Cleck->Vbeta)/2*ts;//u2
	N=(wX > 0.0) + ((wY > 0.0) << 1) + ((wZ > 0.0) << 2);
		  // 根据扇区的不同，计算对应的t_a、t_b和t_c的值，表示生成的三相电压的时间
  if (N == 5)
  {
    // 扇区5的计算
		bSector=1;
    float t4 = wZ;
    float t6 = wX;
    float sum = t4 + t6;
    if (sum > ts)
    {
      float k_svpwm = ts / sum; // 计算缩放系数
      t4 = k_svpwm * t4;
      t6 = k_svpwm * t6;
    }
    float t7 = (ts - t4 - t6) / 2;
    U_spwm = t4 + t6 + t7;
    W_spwm = t6+ t7;
    V_spwm = t7;
  }
  else if (N == 1)
  {
    // 扇区1的计算
		bSector=2;
    float t2 = -wZ;			
    float t6 = -wY;
    float sum = t2 + t6;
    if (sum > ts)
    {
      float k_svpwm = ts / sum; // 计算缩放系数
      t2 = k_svpwm * t2;
      t6 = k_svpwm * t6;
    }
    float t7 = (ts - t2 - t6) /2;
    U_spwm = t6+ t7;
    W_spwm = t2 + t6+ t7;
    V_spwm = t7;
  }
  else if (N == 3)
  {
    // 扇区3的计算
		bSector=3;
    float t2 = wX;
    float t3 = wY;
    float sum = t2 + t3;
    if (sum > ts)
    {
      float k_svpwm = ts / sum; // 计算缩放系数
      t2 = k_svpwm * t2;
      t3 = k_svpwm * t3;
    }
    float t7 = (ts - t2 - t3) / 2;
    U_spwm = t7;
    W_spwm = t2 + t3+ t7;
    V_spwm = t3 + t7;
  }
  else if (N == 2)
  {
    // 扇区2的计算
		bSector=4;
    float t1 = -wX;
    float t3 = -wZ;
    float sum = t1 + t3;
    if (sum > ts)
    {
      float k_svpwm = ts / sum; // 计算缩放系数
      t1 = k_svpwm * t1;
      t3 = k_svpwm * t3;
    }
    float t7 = (ts - t1 - t3) / 2;
    U_spwm = t7;
    W_spwm = t3 + t7;
    V_spwm = t1 + t3 + t7;
  }
  else if (N == 6)
  {
    // 扇区6的计算
		bSector=5;
    float t1 = wY;
    float t5 = wZ;
    float sum = t1 + t5;
    if (sum > ts)
    {
      float k_svpwm = ts / sum; // 计算缩放系数
      t1 = k_svpwm * t1;
      t5 = k_svpwm * t5;
    }
    float t7 = (ts - t1 - t5)/2 ;
    U_spwm = t5 + t7;
    W_spwm = t7;
    V_spwm = t1 + t5 + t7;
  }
  else if (N == 4)
  {
    // 扇区4的计算
		bSector=6;
    float t4 = -wY;
    float t5 = -wX;
    float sum = t4 + t5;
    if (sum > ts)
    {
      float k_svpwm = ts / sum; // 计算缩放系数
      t4 = k_svpwm * t4;
      t5 = k_svpwm * t5;
    }
    float t7 = (ts - t4 - t5) /2;
    U_spwm = t4 + t5 + t7;
    W_spwm = t7;
    V_spwm = t5+ t7;
  }

}
//void Svpwm_2(float UAn_1,float UBn_1,float UCn_1)
//{

//				wA=sin_data*ts;
//				wC=(1.73205080f*cos_data-sin_data)/2*ts;//u3
//				wB=-(1.73205080f*cos_data+sin_data)/2*ts;//u2
//	N2=(wA > 0.0) + ((wB > 0.0) << 1) + ((wC > 0.0) << 2);
//		  // 根据扇区的不同，计算对应的t_a、t_b和t_c的值，表示生成的三相电压的时间
//  if (N2 == 5)
//  {
//    // 扇区5的计算
//		bSector=1;
//    float t4 = wC;
//    float t6 = wA;
//    float sum = t4 + t6;
//    if (sum > ts)
//    {
//      float k_svpwm = ts / sum; // 计算缩放系数
//      t4 = k_svpwm * t4;
//      t6 = k_svpwm * t6;
//    }
//    float t7 = (ts - t4 - t6) / 2;
//    A_spwm = t4 + t6 + t7;
//    C_spwm = t6 + t7;
//    B_spwm = t7;
//  }
//  else if (N2 == 1)
//  {
//    // 扇区1的计算
//		bSector=2;
//    float t2 = -wC;			
//    float t6 = -wB;
//    float sum = t2 + t6;
//    if (sum > ts)
//    {
//      float k_svpwm = ts / sum; // 计算缩放系数
//      t2 = k_svpwm * t2;
//      t6 = k_svpwm * t6;
//    }
//    float t7 = (ts - t2 - t6) / 2;
//    A_spwm = t6 + t7;
//    C_spwm = t2 + t6 + t7;
//    B_spwm = t7;
//  }
//  else if (N2 == 3)
//  {
//    // 扇区3的计算
//		bSector=3;
//    float t2 = wA;
//    float t3 = wB;
//    float sum = t2 + t3;
//    if (sum > ts)
//    {
//      float k_svpwm = ts / sum; // 计算缩放系数
//      t2 = k_svpwm * t2;
//      t3 = k_svpwm * t3;
//    }
//    float t7 = (ts - t2 - t3) / 2;
//    A_spwm = t7;
//    C_spwm = t2 + t3 + t7;
//    B_spwm = t3 + t7;
//  }
//  else if (N2 == 2)
//  {
//    // 扇区2的计算
//		bSector=4;
//    float t1 = -wA;
//    float t3 = -wC;
//    float sum = t1 + t3;
//    if (sum > ts)
//    {
//      float k_svpwm = ts / sum; // 计算缩放系数
//      t1 = k_svpwm * t1;
//      t3 = k_svpwm * t3;
//    }
//    float t7 = (ts - t1 - t3) / 2;
//    A_spwm = t7;
//    C_spwm = t3 + t7;
//    B_spwm = t1 + t3 + t7;
//  }
//  else if (N2 == 6)
//  {
//    // 扇区6的计算
//		bSector=5;
//    float t1 = wB;
//    float t5 = wC;
//    float sum = t1 + t5;
//    if (sum > ts)
//    {
//      float k_svpwm = ts / sum; // 计算缩放系数
//      t1 = k_svpwm * t1;
//      t5 = k_svpwm * t5;
//    }
//    float t7 = (ts - t1 - t5) / 2;
//    A_spwm = t5 + t7;
//    C_spwm = t7;
//    B_spwm = t1 + t5 + t7;
//  }
//  else if (N2 == 4)
//  {
//    // 扇区4的计算
//		bSector=6;
//    float t4 = -wB;
//    float t5 = -wA;
//    float sum = t4 + t5;
//    if (sum > ts)
//    {
//      float k_svpwm = ts / sum; // 计算缩放系数
//      t4 = k_svpwm * t4;
//      t5 = k_svpwm * t5;
//    }
//    float t7 = (ts - t4 - t5) / 2;
//    A_spwm = t4 + t5 + t7;
//    C_spwm = t7;
//    B_spwm = t5 + t7;
//  }

//}




