// 
// 
// 

#include "Communication.h"

static uint8_t handle_cmd(String cmd);

//This is handled, if we got data via Serial1
void handle_comm()
{
	String line = Serial.readStringUntil('\n');

	uint8_t index = line.indexOf(F("AT+"));
	if (index >= 0 ) //AT command detected
	{
		line.trim();
		String cmd = line.substring(index + 3);//, line.length() - 1);		//Cut The command after the AT+ and before the \n 
		
		uint8_t result = AT_ERROR;

		if (cmd.indexOf('=') == -1)
			result = handle_cmd(cmd);

		if (result == AT_OK)
			Serial.println(F("OK"));
		else if (result == AT_UNKNOWN_CMD)
			Serial.println(F("ERROR (Unknown Cmd)"));
		else
			Serial.println(F("ERROR"));
	}
}

//Handle all commands without an =
static uint8_t handle_cmd(String cmd)
{
	if (cmd.equals("")) //"AT+" should return OK only
	{
		return  AT_OK;
	}
	else if (cmd.equalsIgnoreCase(F("VER")) || cmd.equalsIgnoreCase(F("VERSION")))  //Print version number
	{
		Serial.println(VERSION_STR);
		return AT_OK;
	}
	else if (cmd.equalsIgnoreCase(F("INFO"))) //Prints some infos about the software
	{
		Serial.println(INFO_STR);
		return AT_OK;
	}
	else if (cmd.equalsIgnoreCase(F("VOLT")) || cmd.equalsIgnoreCase(F("BATT"))) //Prints some infos about the software
	{
		Serial.print(battery_voltage());
		Serial.println(" mV");
		return AT_OK;
	}
	else if (cmd.equalsIgnoreCase(F("VOLT2")) || cmd.equalsIgnoreCase(F("3V3"))) //Prints some infos about the software
	{
		Serial.print(mcu_voltage());
		Serial.println(" mV");
		return AT_OK;
	}
	else if (cmd.equalsIgnoreCase(F("POWEROFF")))	//Poweroff the AP510
	{
		Serial.println("Shutdown...");
		delay(50); //Wait for output complete
		POWER_OFF();
		return AT_OK;
	}
	else if (cmd.equalsIgnoreCase(F("DEBUG_BEEP_SHORT")))  //Short beep
	{
		BEEP_SHORT();
		return AT_OK;
	}
	else if (cmd.equalsIgnoreCase(F("DEBUG_BEEP_MEDIUM")))  //Medium beep
	{
		BEEP_MEDIUM();
		return AT_OK;
	}
	else if (cmd.equalsIgnoreCase(F("DEBUG_BEEP_LONG")))  //Long beep
	{
		BEEP_LONG();
		return AT_OK;
	}
	else
	{
		return AT_UNKNOWN_CMD;	//Unknown command
	}
}

static uint8_t handle_set()
{
	//TODO
}

static uint8_t handle_get()
{
	//TODO
}