// 
// 
// 

#include "helpers.h"

long battery_voltage()
{
	analogReference(INTERNAL);
	delay(50);	//Wait for settle
	uint16_t adc = analogRead(VOLT_MEASURE);
	long voltage = adc * 2560L / 1024L;
	voltage = voltage * VOLT_CALIB / 1000;	//The ADC input is on a 1:10 divider
	
	return voltage;
}

long mcu_voltage() 
{
	long result; 
	// Read 1.1V reference against AVcc 
	ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1); 
	delay(2); 
	// Wait for Vref to settle 
	ADCSRA |= _BV(ADSC); 
	// Convert 
	while (bit_is_set(ADCSRA,ADSC)); 
	result = ADCL; result |= ADCH<<8; 
	result = ADC_Vbg * 1024L / result; // Back-calculate AVcc in mV 
	return result; 
}
