#include "Ave_Sam.h"
#include "Svpwm.h"


float Ave_i[200]={0};
float Ave_u[200]={0};
float Ave_w[200]={0};
float	   u_sum,i_sum,u_ref_,i_ref_,w_sum,w_ref_;

int n,Ave_flag;
float v_s1,v0_s_1,v1_s_1,v2_s_1,v3_s_1,K=1.414,W=2*PI*50,/*输入电压的频率*/Ts=0.0001,/*10k中断频率*/sin_Theta,cos_Theta;

float Ave_Sam_u(float u_in,int Sam_num)
{
  
	if(Ave_flag==1)
		{
			Ave_u[n]=u_in;
		}
		if(Ave_flag==0)
		{
			Ave_u[n]=u_in;
			n++;
			
		}
		
		if(n==Sam_num || Ave_flag==1)
		{
			
			for(n=0;n<Sam_num;n++)
			{
				u_sum=Ave_u[n]+u_sum;
				
			}
			u_ref_=u_sum / Sam_num;
			for(n=0;n<Sam_num;n++)
			{
				Ave_u[n]=Ave_u[n+1];
			}
			u_sum=0;
			Ave_flag=1;
		}
		return u_ref_;
}






float Ave_Sam_i(float i_in,int Sam_num)
{
  
	if(Ave_flag==1)
		{
			Ave_i[n]=i_in;
		}
		if(Ave_flag==0)
		{
			Ave_i[n]=i_in;
			n++;
			
		}
		
		if(n==Sam_num || Ave_flag==1)
		{
			
			for(n=0;n<Sam_num;n++)
			{
				i_sum=Ave_i[n]+i_sum;
				
			}
			i_ref_=i_sum / Sam_num;
			for(n=0;n<Sam_num;n++)
			{
				Ave_i[n]=Ave_i[n+1];
			}
			i_sum=0;
			Ave_flag=1;
		}
		return i_ref_;
}





float Ave_Sam_w(float i_in,int Sam_num)
{
  static float Ave_w[200]={0};
	if(Ave_flag==1)
		{
			Ave_w[n]=i_in;
		}
		if(Ave_flag==0)
		{
			Ave_w[n]=i_in;
			n++;
			
		}
		
		if(n==Sam_num || Ave_flag==1)
		{
			
			for(n=0;n<Sam_num;n++)
			{
				w_sum=Ave_w[n]+w_sum;
				
			}
			w_ref_=w_sum / Sam_num;
			for(n=0;n<Sam_num;n++)
			{
				Ave_w[n]=Ave_w[n+1];
			}
			w_sum=0;
			Ave_flag=1;
		}
		return w_ref_;
}
float sogi(float Sin_data)
{
		float Theta_1;
		////////////输入需要鉴相的三角函数给v_s1
		v0_s_1 = ((Sin_data-v1_s_1)*K-v2_s_1)*W;
		v1_s_1 += v0_s_1*Ts;//V_s1的阿尔法轴的电压,相当于sin（）
		v3_s_1 += v1_s_1*Ts;
		v2_s_1 = v3_s_1*W;//V_s1的β轴的电压,滞后90°，相当于-cos（）
		sin_Theta=v1_s_1;
		cos_Theta=-v2_s_1;
				if(sin_Theta>0&&cos_Theta>0)
		{
			Theta_1=atan(sin_Theta/cos_Theta);
		}
		if(sin_Theta<0&& cos_Theta>0)
		{
			Theta_1=atan(sin_Theta/cos_Theta);
		}
		if(sin_Theta>0 && cos_Theta<0)
		{
			Theta_1=PI - atan((sin_Theta)/(-cos_Theta));
		}
		if(sin_Theta<0 && cos_Theta<0)
		{
			Theta_1=PI + atan((-sin_Theta)/(-cos_Theta));
		}
		return Theta_1;
}
float quzheng(float w)
{
	return w>0?w:-w;
}


