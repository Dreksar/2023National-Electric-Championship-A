#ifndef __SOGI_PLL_H
#define __SOGI_PLL_H
#include "arm_math.h"
// 以下是各种参数调整细节，酌情调节

// 锁相环
#define wn (2.0f * PI * 50.0f)   // 频率为50赫兹
#define wc (2.0f * PI * 20.0f)   // 自然频率，可以根据实际情况调节，建议调小点
#define Vm (2.2f)                // 电压采样幅值，根据实际情况调节，锁相环的PID参数使用
#define kz (1.414f)              // 阻尼系数的两倍，一般阻尼系数取0.707
#define Ts (1.0f / 20000.f)        // 15K赫兹的采样周期，1/15000
#define xx (2.0f * kz * wn * Ts) // x系数，用于离散化方程系数
#define yy (wn * wn * Ts * Ts)   // y系数，用于离散化方程系数
#define Q_ki (wc * wc / Vm)      // 锁相环KI参数
#define Q_kp ((kz * Q_ki) / wc)  // 锁相环KP参数
// 电压环
#define R (50)                      // 负载阻值
#define C (0.000001f * 2670)        // 电容容值
#define V_ki (C * wc * wc)          // 电压环KI参数
#define V_kp (-1 / R + kz * C * wc) // 电压环KP参数
#define loop (500)                  // 电压环响应速度
// 电流环
#define I_kp (0.0010f) // 电流环KP参数
#define Kr (3000.0f)   // 电流环KR参数
#define wr (1.5f * PI) // 电流环带宽
// SOGI差分方程系数
#define aa1 (xx / (xx + yy + 4.0f))
#define aa3 (-xx / (xx + yy + 4.0f))
#define aa4 (2.0f * (4.0f - yy) / (xx + yy + 4.0f))
#define aa5 ((xx - yy - 4.0f) / (xx + yy + 4.0f))
#define bb1 (kz * yy / (xx + yy + 4.0f))
#define bb2 (2.0f * kz * yy / (xx + yy + 4.0f))
#define bb3 (kz * yy / (xx + yy + 4.0f))
#define bb4 (2.0f * (4.0f - yy) / (xx + yy + 4.0f))
#define bb5 ((xx - yy - 4.0f) / (xx + yy + 4.0f))
// 简单积分器系数
#define aai (Ts / 2.0f)
// SOGI的PID离散化参数
#define Q_k0 (Q_kp + Q_ki * Ts)
#define Q_k1 (-Q_kp)
// 电压环的PID离散化参数
#define V_k0 (0.05f)
#define V_k1 (0.0001f)
// 逆变侧 PID参数
#define S_k0 (0.002f)
#define S_k1 (0.001f)
// 电流环的各项离散化参数
#define cc1 ((4.0f * I_kp + Ts * Ts * I_kp * wn * wn + 4.0f * Ts * I_kp * wr + 4.0f * Ts * Kr * wr) / (Ts * Ts * wn * wn + 4.0f * wr * Ts + 4.0f))
#define cc2 ((2.0f * I_kp * Ts * Ts * wn * wn - 8.0f * I_kp) / (Ts * Ts * wn * wn + 4.0f * wr * Ts + 4.0f))
#define cc3 ((4.0f * I_kp + Ts * Ts * I_kp * wn * wn - 4.0f * Ts * I_kp * wr - 4.0f * Ts * Kr * wr) / (Ts * Ts * wn * wn + 4.0f * wr * Ts + 4.0f))
#define cc4 ((2.0f * Ts * Ts * wn * wn - 8.0f) / (Ts * Ts * wn * wn + 4.0f * wr * Ts + 4.0f))
#define cc5 ((Ts * Ts * wn * wn + 4.0f - 4.0f * Ts * wr) / (Ts * Ts * wn * wn + 4.0f * wr * Ts + 4.0f))
#define coeffIac (1.90f)   // 交流电流转换系数，采样值乘以系数为真实电流值
#define coeffVac (34.883f) // 交流电压转换系数，采样值乘以系数为真实电压值
#define coffVdc (30.0f)    // 直流电压转换系数
// 陷波器离散化参数
#define ws (2.0f * PI * 100.0f)  // 陷波频率
#define dep (0.01f)              // 陷波深度  衰减倍数
#define wb (2.0f * PI * 10.0f)   // 陷波带宽
#define ks1 (0.049942766363377f) //(arm_sqrt_f32((1.0f-arm_sqrt_f32(1.0f+((wb*wb)/(ws*ws))))/(4.0f*dep*dep-2.0f)))
#define ks2 (dep * ks1)
#define ka1 (ws * ws * Ts * Ts + 4.0f * Ts * ks1 * ws + 4.0f)
#define ka2 (2.0f * ws * ws * Ts * Ts - 8.0f)
#define ka3 (ws * ws * Ts * Ts - 4.0f * Ts * ks1 * ws + 4.0f)

#define kb1 (ws * ws * Ts * Ts + 4.0f * Ts * ks2 * ws + 4.0f)
#define kb2 (2.0f * ws * ws * Ts * Ts - 8.0f)
#define kb3 (ws * ws * Ts * Ts - 4.0f * Ts * ks2 * ws + 4.0f)
#define SOGI_U_Init                                                  \
    {                                                                \
        aa1, 0.0f, aa3, aa4, aa5, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f \
    }
#define SOGI_QU_Init                                                \
    {                                                               \
        bb1, bb2, bb3, bb4, bb5, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f \
    }
#define SOGI_QUPID_Init                                                             \
    {                                                                               \
        0.0f, Q_k0, Q_k1, 0.0f, 400.0f, 300.0f, 0.0f, 50.0f, 60.0f, 400.0f, -400.0f \
    }
#define SOGI_INTE_Init              \
    {                               \
        aai, 0.0f, 0.0f, 0.0f, 0.0f \
    }
#define PIV_Init                                                                 \
    {                                                                            \
        60.0f, V_k0, V_k1, 0.0f, 0.0f, 10.0f, 20.0f, 2.0f, 2.0f, 3.500f, 0.0001f \
    }
#define PRI_Init                                                                       \
    {                                                                                  \
        cc1, cc2, cc3, cc4, cc5, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2000.0f, -2000.0f \
    }
#define PIsin_Init                                                            \
    {                                                                         \
        30.0f, S_k0, S_k1, 0.0f, 0.0f, 0.0f, 0.0f, 0.850f, 0.80f, 0.82f, 0.5f \
    }
typedef struct // SOGI结构体，用于差分生成U与滞后90°的qU
{
    float a1;
    float a2;
    float a3;
    float a4;
    float a5;
    float x0; // 此次采集电压值
    float x1; // 上一次采集到的电压
    float x2; // 上二次采集到的电压
    float y0; // 滤波后的电压
    float y1; // 上一次滤波后的电压
    float y2; // 上二次滤波后的电压
} SOGI;
typedef struct // 积分器，PI调节器输出后积分即为锁相环相位
{
    float ai;
    float x0;
    float x1;
    float y0;
    float y1;
} inte;
typedef struct
{
    float Ref; // 输入：系统待调节量的给定值
    // PID控制器部分
    float k0;     // 变量：a0
    float k1;     // 变量: a1
    float k2;     // 变量: a2
    float Err;    // 变量：当前的偏差e(k)
    float Err_1;  // 历史：前一步的偏差e(k-1)
    float Err_2;  // 历史：前前一步的偏差e(k-2)
    float Out;    // 输出：PID控制器的输出u(k)
    float Out_1;  // 历史：PID控制器前一步的输出u(k-1)
    float OutMax; // 参数：PID控制器的最大输出
    float OutMin; // 参数：PID控制器的最小输出
} PID;
// PR 控制器
typedef struct
{
    float k0; // 差分方程各个系数
    float k1;
    float k2;
    float k3;
    float k4;
    float y0; // 输出
    float y1;
    float y2;
    float x0; // 输入为测量值-给定值
    float x1;
    float x2;
    float outmax; // 输出限幅
    float outmin;
} PR;
typedef struct
{
    float angle; // 锁相环输出角度
    float sine;  // 基于锁相环输出角度生成的正弦
    float cose;  // 基于锁相环输出角度生成的余弦
    float Alpha; // Alpha轴
    float Beta;  // Beta轴
    float D;     // D轴
    float Q;     // Q轴
    float an;    // cos输出
} PLL;
typedef struct
{
    float U_inte;
    float I_inte;
    float P_inte;
    float Urms;
    float Irms;
    float T;
    float P_y;
    float P_s;
    float pfc;
} PFC;

typedef struct{
float v0_s;
float v1_s;
float v2_s;
float v3_s;
float d;
float q;
}Fil_TypeDef;
void SOGI_FIL(float x,Fil_TypeDef (*fil));
void PRcon(float err);            // 电流环
void PIDcon(PID *P, float rel);   // PID控制器
void Sogi_pll(float vol,Fil_TypeDef *fil_u);         // 锁相环
void pfctt(float vac, float iac); // 功率因数测量
#endif
