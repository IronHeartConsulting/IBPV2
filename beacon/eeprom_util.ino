#include "config.h"
#include "beacon.h"
#include <EEPROM.h>

// EEPROM utilities

// get the slot ID from the EEPROM.  Validate and return
byte eeprom_slotid() {

	byte slotid;
	slotid = EEPROM.read(EEProm_slotID);
	if ( (slotid >= 0) && (slotid <= MAX_SLOTID) ) {
		return slotid;
	}
	// else...
	return 255; // slotid in eeprom invalid, or eeprom busted
}

int set_eeprom_slotid(byte slotid) {

	if ( (slotid >= 0) && (slotid <= MAX_SLOTID) ) {
		debug_println(slotid);
		EEPROM.write(EEProm_slotID,slotid);
		return 0;
	}
	// else... return error
	return -1;
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
