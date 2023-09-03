#include "SOGI_PLL.h"
SOGI u = SOGI_U_Init;
SOGI qu = SOGI_QU_Init;
inte i = SOGI_INTE_Init;
PID PIQ = SOGI_QUPID_Init;
PLL p = {0.0f};
PID PIV = PIV_Init;
PR PRII = PRI_Init;
PFC pf = {0.0f};
PID PIsin = PIsin_Init;
float phase1 = 0.0f;
#define K       1
#define W           100*PI

//phase1 = 0.4 -> PF = 0.5
//phase1 = 0.35 -> PF = 0.59
//phase1 = 0.3 -> PF = 0.69
//phase1 = 0.25 -> PF = 0.78
//phase1 = 0.2 -> PF = 0.69
//phase1 = 0.15 -> PF = 0.89
//phase1 = 0.1 -> PF = 0.91
//phase1 = 0.05 -> PF = 0.91
//phase1 = 0.0 -> PF = 0.93
//phase1 = -0.1 -> PF = 0.91


// 派克变换
static void Park()
{
    p.D = p.Alpha * p.cose + p.Beta * p.sine;
    p.Q = p.Beta * p.cose - p.Alpha * p.sine;
}
// SOGI滤波器(用于生成DQ轴的同相位和滞后90°相位)
static void BTF(SOGI *U, float vol)
{
    U->x0 = vol;
    U->y0 = U->a1 * U->x0 + U->a2 * U->x1 + U->a3 * U->x2 + U->a4 * U->y1 + U->a5 * U->y2;
    U->y2 = U->y1;
    U->y1 = U->y0;
    U->x2 = U->x1;
    U->x1 = U->x0;
}
// PID控制器(增量式建议不用Kd)
void PIDcon(PID *P, float rel)
{
    P->Err = P->Ref - rel;
    P->Out = P->Out_1 + P->k0 * P->Err + P->k1 * P->Err_1 + P->k2 * P->Err_2;
    if (P->Out > P->OutMax)
        P->Out = P->OutMax;
    if (P->Out < P->OutMin)
        P->Out = P->OutMin;
    P->Out_1 = P->Out;
    P->Err_1 = P->Err;
    P->Err_2 = P->Err_1;
}
// PR控制器
void PRcon(float err)
{
    PRII.x0 = err;
    PRII.y0 = PRII.k0 * PRII.x0 + PRII.k1 * PRII.x1 + PRII.k2 * PRII.x2 - PRII.k3 * PRII.y1 - PRII.k4 * PRII.y2;
    if (PRII.y0 > PRII.outmax)
        PRII.y0 = PRII.outmax;
    else if (PRII.y0 < PRII.outmin)
        PRII.y0 = PRII.outmin;
    PRII.x2 = PRII.x1;
    PRII.x1 = PRII.x0;
    PRII.y2 = PRII.y1;
    PRII.y1 = PRII.y0;
}
// 简单积分器
static void integ(float ii)
{
    i.x0 = 100 * PI + ii;
    i.y0 = i.ai * i.x0 + i.ai * i.x1 + i.y1;
    if (i.y0 > 2.0f)
        i.y0 = i.y0 - 2.0f;
    else if (i.y0 < -2.0f)
        i.y0 = i.y0 + 2.0f;
    i.y1 = i.y0;
    i.x1 = i.x0;
}
// 基于SOGI滤波器的dq锁相环
void Sogi_pll(float vol,Fil_TypeDef *fil_u)
{
    p.sine = arm_sin_f32(p.angle * PI);
    p.cose = arm_cos_f32(p.angle * PI);
    BTF(&u, vol);
    BTF(&qu, vol);
    p.Alpha = u.y0;
    p.Beta = qu.y0;
	 fil_u->q = arm_sin_f32(p.angle* PI)*(-1)*p.Alpha + arm_cos_f32(p.angle* PI)*p.Beta;
   fil_u->d= -(arm_cos_f32(p.angle* PI)*p.Alpha+ arm_sin_f32(p.angle* PI)*p.Beta);
    Park();
    PIDcon(&PIQ, p.Q);
    integ(PIQ.Out);
    p.angle = i.y0+phase1;
    p.an = arm_cos_f32((p.angle ) * PI);
}
// 功率因数测量
void pfctt(float vac, float iac)
{
    pf.U_inte = pf.U_inte + (vac * vac);
    pf.I_inte = pf.I_inte + (iac * iac);
    pf.P_inte = pf.P_inte + (vac * iac);
    if (pf.T > 2000.0f)
    {
        arm_sqrt_f32((pf.U_inte / 2000.0f), &pf.Urms);
        arm_sqrt_f32((pf.I_inte / 2000.0f), &pf.Irms);
        pf.P_y = pf.P_inte / 2000.0f;
        pf.P_s = pf.Urms * pf.Irms;
        pf.pfc = pf.P_y / pf.P_s;
        pf.U_inte = 0.0f;
        pf.I_inte = 0.0f;
        pf.P_inte = 0.0f;
        pf.T = 0.0f;
    }
    pf.T++;
}

void SOGI_FIL(float x,Fil_TypeDef (*fil))
{
     fil->v0_s = ((x-fil->v1_s)*K-fil->v2_s)*W;
     fil->v1_s += fil->v0_s*Ts;
     fil->v3_s += fil->v1_s*Ts;
     fil->v2_s = fil->v3_s*W;
     fil->q = arm_sin_f32(p.angle* PI)*(-1)*fil->v1_s + arm_cos_f32(p.angle* PI)*fil->v2_s;
     fil->d = arm_cos_f32(p.angle* PI)*fil->v1_s + arm_sin_f32(p.angle* PI)*fil->v2_s;
}