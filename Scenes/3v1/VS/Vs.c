#ifndef CUSTOM_VS_C
#define CUSTOM_VS_C

#include "Vs.h"
#include "../../../Common.h"
#include "../../3v1/CustomGame.h"

void minor_think() {
	// 3v1
	if (IsCustomMode()) {
		if (*stc_frame_count == 0) {
			Fighter_SetStocks(0, 1);
		}
	}

	VS_Think();
	return;
}

void minor_load(MatchInit* minor_data) {
	OSReport("Loading VS\n");
	// OSReport("Current Major: %d\n", Scene_GetCurrentMajor);
	VS_Load(minor_data);
	
	return;
}

void minor_exit(ScDataVictory* minor_data) {
	VS_Exit(minor_data);

	return;
}


#endif


