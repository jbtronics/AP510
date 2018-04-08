// helpers.h

#ifndef _HELPERS_h
#define _HELPERS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "pin_defines.h"
#include "config.h"

/*******************
* Makros
********************/
#define POWER_ON()		bitSet(DDRA, 6); bitSet(PORTA, 6)	//Activate Power Supply
#define POWER_OFF()		bitClear(PORTA, 6)					//Disables Power Supply (including the MCU!)

#define BEEP_SHORT()	tone(BUZZER, BEEP_FREQ, BEEP_DUR_SHORT)
#define BEEP_MEDIUM()	tone(BUZZER, BEEP_FREQ, BEEP_DUR_MEDIUM)
#define BEEP_LONG()		tone(BUZZER, BEEP_FREQ, BEEP_DUR_LONG)
#define BEEP_XLONG()	tone(BUZZER, BEEP_FREQ, BEEP_DUR_XLONG)

/******************
* Functions
*******************/

//Returns the battery voltage in mV
long battery_voltage();
long mcu_voltage();


#endif

