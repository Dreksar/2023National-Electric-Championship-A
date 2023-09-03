#ifndef _VOFA_H
#define _VOFA_H
#include "struct_typedef.h"

typedef struct
{
		float data[2];
		uint8_t tail[4];
}Vofa_data2;


typedef struct
{
		float data[4];
		uint8_t tail[4];
}Vofa_data4;




void send_data_to_vofa2(float data1, float data2);
void send_data_to_vofa4(float data1, float data2, float data3, float data4);

#endif
