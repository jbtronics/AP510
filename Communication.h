// Communication.h

#ifndef _COMMUNICATION_h
#define _COMMUNICATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "helpers.h"

void handle_comm();

#define AT_OK 0
#define AT_ERROR 1
#define AT_UNKNOWN_CMD 2

#endif

