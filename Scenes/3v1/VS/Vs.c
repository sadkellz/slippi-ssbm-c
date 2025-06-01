#ifndef CUSTOM_VS_C
#define CUSTOM_VS_C

#include "Vs.h"
#include "../../../Common.h"

SharedMinorData* data;

void minor_think() {
	VS_Think();
	return;
}

void minor_load(MatchInit* minor_data) {
	bp();
	minor_data->playerData[0].stocks = 1;
	VS_Load(minor_data);
	
	return;
}

void minor_exit(ScDataVictory* minor_data) {
	VS_Exit(minor_data);

	return;
}


#endif


