#include "Arduino.h"
#include "Beacon_Types.h"



BCNDebug::BCNDebug()
{
	//  constructor code goes here
}

// turn on or off various debugging flags
void BCNDebug::RS232DebugON()
{
	debugFlag |= dbgRS232ON;
}
void BCNDebug::RS232DebugOFF()
{
	debugFlag &= ~dbgRS232ON;
}

// PrintDebug - pointer to data to print, number of bytes to print, and a message to proceed the data
// will only print if an appropirate debug flag is on
void BCNDebug::PrintDebug(uint8_t *data, uint8_t length, char *message)
{
	if ( (debugFlag & dbgRS232ON) == dbgRS232ON) 
		PrintHex83(data, length, message);
}

/************************************************/
/*	FORMAT BYTE DATA FOR PRINTING				*/
/*	Purpose: format data to print byte format	*/
/*			with leading 0.  For example:		*/
/*			0xF becomes 0x0F					*/
/************************************************/
void BCNDebug::PrintHex83(uint8_t *data, uint8_t length, char *message)
{
	char cdata[length * 2 + 1];
	byte first ;
	int j=0;
	for (uint8_t i=0; i<length; i++)
	{
		first = (data[i] >> 4) | 48;	//Shift right 4 bits and move it in ASCII to numeric
		if (first > 57) 				//If number is greater than 9 then do something
			cdata[j] = first + (byte)7;	//Move number to upper case alphabetical area of ASCII table
		else 
			cdata[j] = first ;			//If number is 0 - 9, then done.
		j++;

		first = (data[i] & 0x0F) | 48;	//Process lower 4 bits, move it to numeric part of ASCII
		if (first > 57) 				//If the resulting number is greater than 9, do more processing
			cdata[j] = first + (byte)7;	//Move number to upper case alphabetical area of ASCII table
		else 
			cdata[j] = first;			//If number is 0 - 9, then done
		j++;
	}
	cdata[length*2] = 0;				//Make sure to add a terminating character at the end.
	Serial.print(message);
	Serial.println(cdata);
} 
