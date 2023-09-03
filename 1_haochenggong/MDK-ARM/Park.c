#include "park.h"
#include "cleck.h"

float theta_sin;
float theta_cos;


void anti_park(PARK *Park,CLECK *Cleck)
{
	theta_cos=cosf(Park->theta);
	theta_sin=sinf(Park->theta);
	Cleck->Valpha=Park->Vd*theta_cos-Park->Vq*theta_sin;
	Cleck->Vbeta =Park->Vd*theta_sin+Park->Vq*theta_cos;
}

void park(PARK *Park,CLECK *Cleck)
{
	theta_cos=cosf(Park->theta);
	theta_sin=sinf(Park->theta);
	Park->Id=Cleck->Ialpha*theta_cos+Cleck->Ibeta*theta_sin;
	Park->Iq=-Cleck->Ialpha*theta_sin+Cleck->Ibeta*theta_cos;	
}


