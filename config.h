// config_h.h

#ifndef _CONFIG_h
#define _CONFIG_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


//Beep tone config
#define BEEP_FREQ			1000	//1kHz
#define BEEP_DUR_SHORT		30
#define BEEP_DUR_MEDIUM		100
#define BEEP_DUR_LONG		200
#define BEEP_DUR_XLONG		500

//Button debounce
#define BUTTON_DEBOUNCE_DELAY   20   // [ms]

//Times
#define OFF_DURATION		2000	//How long the button must be hold


#define VOLT_CALIB			4291L	//	(Vin/Vout) * 1000, Default is 4023L
#define ADC_Vbg				1057L	// Bandgap voltage in mv (Default 1230mV)

#define VERSION_STR			F("0.1-dev")
#define INFO_STR			F("AP510 Firmware by DO9JHB")

#endif

