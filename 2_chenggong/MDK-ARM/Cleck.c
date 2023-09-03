#include "cleck.h"

float sin_30;
float cos_30;
void anti_cleck(CLECK *Cleck)
{
	sin_30=sinf(PI/6.0f);
	cos_30=cosf(PI/6.0f);
	Cleck->Va=Cleck->Valpha;
	Cleck->Vb=-Cleck->Valpha*sin_30+cos_30*Cleck->Vbeta;
	Cleck->Vc=-Cleck->Valpha*sin_30-cos_30*Cleck->Vbeta;
}

void cleck(CLECK *Cleck)
{
	sin_30=sinf(PI/6.0f);
	cos_30=cosf(PI/6.0f);
	Cleck->Ialpha=Cleck->Ia;
	Cleck->Ibeta=(Cleck->Ia+2*Cleck->Ib)/sqrt(3.0f);
}