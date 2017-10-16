#include "config.h"
#include "beacon.h"
#include <EEPROM.h>

// EEPROM utilities

// get the slot ID from the EEPROM.  Validate and return
byte eeprom_slotid() {

	byte slotid = 255;
	slotid = EEPROM.read(EEProm_slotID);
	if ( (slotid >= 0) && (slotid < 18) ) {
		return slotid;
	}
	// else...
	return 255; // slotid in eeprom invalid, or eeprom busted
}

// read the delay value.
//  stored as a little endin two byte sequence
///  re-assemble and return a value
uint16_t eepromDelay() {
	uint16_t result;
	result = EEPROM.read(EEProm_delayL);
	result += EEPROM.read(EEProm_delayH)*256;
	return(result);
}

// dump the content of the EEPROM - for debugging.  Goes to serial console only
void dump_eeprom() {
	byte used = 0xff;   // count of bytes to print
	byte value;
	unsigned int i;

	used = EEPROM.read(EEProm_used);
	Serial.print(F("EEPROM len:"));
	Serial.println(used);
	if (used < 7) used = 10;  // dump at least the header
	for (i=0; i < used; i++) {
		value = EEPROM.read(i);
		Serial.print(i,HEX);
		Serial.print(":");
		Serial.println(value,HEX);
	}
}
