// RF.h

#ifndef _RF_h
#define _RF_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define BUFFER_SIZE 32

void rf_init();
void rf_setFreq(long Hz);

void rf_send_config();

static void timer_init();

void rf_write(uint8_t data[BUFFER_SIZE]);

#define START_SEND()	 digitalWrite(RF_PTT, HIGH)
#define END_SEND()		 digitalWrite(RF_PTT, LOW)

#define RFSerial		Serial1

//Config Defines
#define BANDWIDTH_N		0
#define BANDWIDTH_W		1

#define POWER_HIGH		0
#define POWER_LOW		1

#define AFSK_ON()		TCNT3 = 0; TCCR3A = (1 << COM3C0)		//Activate output of the AFSK signal
#define AFSK_OFF()		TCCR3A &= ~(1 << COM3C0)		//Deactivate output of the AFSK signal
#define AFSK_1200()		OCR3A = 191; OCR3C = 191			//Output 1200Hz tone
#define AFSK_2200()		OCR3A = 104; OCR3C = 104			//Output 2200Hz tone

#define AFSK_TIMER_START() TCCR2 |= (1 << CS02) | (1 << CS00)
#define AFSK_TIMER_RESET() TCNT2 = 244
#define AFSK_TIMER_STOP() TCCR2 = 0;

#define STATE_1200		0
#define STATE_2200		1

#endif

