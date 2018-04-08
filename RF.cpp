// 
// 
// 

#include "RF.h"
#include "helpers.h"

struct freq_t {
	uint8_t mhz;
	uint16_t khz;
};

struct RF_CONFIG 
{
	uint8_t bandwidth;
	float tx_freq;
	float rx_freq;

	uint8_t power;
} config;

void rf_init()
{
	//Handshake with Module
	RFSerial.println("AT+DMOCONNECT");

	config.bandwidth = BANDWIDTH_N;
	config.tx_freq = 144.900;
	config.rx_freq = 144.900;
	
	config.power = POWER_LOW;

	//Configure Control IOs for Output
	pinMode(RF_PTT, OUTPUT);
	pinMode(RF_PD, OUTPUT);

	digitalWrite(RF_PD, HIGH); //Deactivate sleep mode
	delay(500); //We need this delay so 

	rf_send_config();

	timer_init();
}

void rf_send_config()
{
	RFSerial.print("AT+DMOSETGROUP=");
	RFSerial.print(config.bandwidth);
	RFSerial.print(",");
	RFSerial.print(config.tx_freq, 4); //The freq settings need 4 digits after the point
	RFSerial.print(",");
	RFSerial.print(config.rx_freq, 4);
	RFSerial.print(",");

	RFSerial.print("0");	//No RXCTCSS
	RFSerial.print(",");
	RFSerial.print("2");		//Squelch level
	RFSerial.print(",");
	RFSerial.print("0");	//No TXCTCSS
	RFSerial.print(",");
	
	uint8_t flags = 0x00;
	bitWrite(flags, 2, config.power);	//Bit 2 is for power selection
	RFSerial.println("1");
}

void rf_setFreq(long freq)
{
	//config.rx_freq = freq;
	//config.tx_freq = freq;

	rf_send_config();
}

volatile byte buffer;

static void timer_init() //Initialize Timer2 for AFSK send interrupt, Period: 0.0008333s => 1200Bd
{
	//pinMode(RF_MIC, OUTPUT);
	
	DDRE |= (1 << PINE5);		//Set Mic as Output

	/* UART Timer */
			//Prescaler 1024
	TCNT2 = 244;							//Preload
	TIMSK |= (1 << TOIE2);					//Activate Timer2 Overflow interrupt

	// Init Tone timer; MIC is on OCR3C
	TCNT3 = 0;		//Init Timer2
	TCCR3B = (1 << CS31) | (1 << CS30);		//Prescaler 64 on T3
	TCCR3B |= (1 << WGM32);					//CTC Mode
	
	OCR3A = 95;
	OCR3C = 95;

}



static uint8_t is_sending;
static uint8_t send_byte[BUFFER_SIZE];

void rf_write(uint8_t data[BUFFER_SIZE])
{
	*send_byte = *data;
	is_sending = true;

	AFSK_TIMER_RESET();
	AFSK_TIMER_START();
}

ISR(TIMER2_OVF_vect)
{
	static uint8_t bit_pos = 0;
	static uint8_t byte_pos = 0;
	TCNT2 = 244;							//Preload for next cycle

	static uint8_t last = STATE_1200;
	
	uint8_t tmp = bitRead(send_byte[byte_pos], bit_pos);
	
	if (tmp == 0)	//if the bit is 0: Change tone
	{
		if (last == STATE_1200)
		{
			AFSK_2200();
			last = STATE_2200;
		}
		else if (last == STATE_2200)
		{
			AFSK_1200();
			last = STATE_1200;
		}
	}

	bit_pos++;	//Send next bit

	if (bit_pos > 7) //If we reach the end of a byte
	{
		bit_pos = 0; //Begin at the start of byte again
		byte_pos++;	//Next byte from buffer
		if (byte_pos > BUFFER_SIZE)	//If we reach the end of the buffer
		{
			Serial.println("Buffer end reached");
			is_sending = 0;		//Stop sending
			AFSK_TIMER_STOP();	//Stop counter
			AFSK_TIMER_RESET();
			AFSK_OFF();

			bit_pos = 0;	//Reset positions
			byte_pos = 0;
		}
	}

}
