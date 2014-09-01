/*  BCNDebug.h  - Beacon Debug header file
**
**
*/
#ifndef BCNDebug_h
#define BCNDebug_h
#include "Arduino.h"

// debugging flag values
#define dbgRS232ON  0x01;
#define dbgRS232OFF 0x00;

class BCNDebug
{
	public:
		BCNDebug();	// Init Beacon Debug class
		RS232DebugON(); // turn on serial1 port debugging
		RS232DebugOFF(); // stop debugging serial port
		PrintDebug(uint8_t *pdata; uint8_t data_length; char *message);
	private:
		byte debugFlag;	//selected items to debug
		void PrintHex83(uint8_t *data, uint8_t length, char *message);
};

#endif
