#ifndef CUSTOM_SSS_H
#define CUSTOM_SSS_H

#include "../../../m-ex/MexTK/mex.h"

void (*SSS_Think)() = (void *) 0x8025B850;
void (*SSS_Load)(MatchInit* minor_data) = (void *) 0x8025A998;
void (*SSS_Exit)(ScDataVictory* minor_data) = (void *) 0x8025BB5C;

#endif