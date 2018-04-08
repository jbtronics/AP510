#include "Communication.h"
#include "RF.h"
#include <InputDebounce.h>
#include "config.h"
#include "helpers.h"
#include "pin_defines.h"

InputDebounce btn;


void setup()
{
	POWER_ON();
	pinMode(LED, OUTPUT);
	pinMode(BUZZER, OUTPUT);
	Serial.begin(4800);
	Serial1.begin(9600);

	//Setup Button debounce
	btn.registerCallbacks(button_pressedCallback, button_releasedCallback, button_pressedDurationCallback);
	btn.setup(BUTTON, BUTTON_DEBOUNCE_DELAY);

	Serial.println("Power Up");

	rf_init();


	digitalWrite(LED, HIGH);

	/*
	TCCR3A = (1 << COM3C0);
	DDRE |= (1 << 3);
	TCCR3B |= (1 << WGM32) | (1 << CS31) | (1 << CS30);
	//OCR3C = 13900;
	OCR3A = 13900;
	//OCR3B = 13900;
	*/
}

void loop()
{
	btn.process(millis());
	
	static long lasttime;

	if (Serial.available() > 0)
	{
		handle_comm();
	}

#ifdef DEBUG_GPS
	while (Serial1.available())
	{
		Serial.print((char) Serial1.read());
	}
#endif
}

void button_pressedCallback()
{
	//BEEP_MEDIUM();
	START_SEND();
	delay(10);
	AFSK_ON();
	AFSK_TIMER_RESET();
	AFSK_TIMER_START();
	//Serial.println("PTT high");

	uint8_t tmp[64];

	for (uint8_t i = 0; i < BUFFER_SIZE; i++)
	{
		tmp[i] = i;
	}

	rf_write(tmp);

	delay(2000);

	END_SEND();
	
	//Serial.println("PTT low");
}

void button_releasedCallback()
{
	
}

void button_pressedDurationCallback(unsigned long duration)
{
	static uint8_t is_off = false;
	if (duration > OFF_DURATION && !is_off) //When the button is pressed long enough, then switch device off
	{
		is_off = true;
		BEEP_XLONG();
		Serial.println("Shutdown...");
		POWER_OFF();
	}
}