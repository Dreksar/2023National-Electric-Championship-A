#ifndef _SVPWM_H
#define _SVPWM_H

#include "Svpwm.h"
#include "main.h"
#include "Cleck.h"
#include "Park.h"
#define SECTOR_1      1
#define SECTOR_2      2
#define SECTOR_3      3
#define SECTOR_4      4
#define SECTOR_5      5
#define SECTOR_6      6
#define ts            (4200*2)

#define sqrt_2       1.4142135f
#define sqrt_3       1.73205080f


void Sin_Start(uint16_t Frquence,float k,float Vdc);
void Svpwm(CLECK *Cleck,float a,float b,float c);
#endif