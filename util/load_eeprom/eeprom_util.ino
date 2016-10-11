#include "config.h"
#include "beacon.h"
#include <EEPROM.h>

// EEPROM utilities

// get the slot ID from the EEPROM.  Validate and return
byte eeprom_slotid() {

	byte slotid = 255;
	slotid = EEPROM.read(EEProm_slotID);
	if ( (slotid > 0) && (slotid < 18) ) {
		return slotid;
	}
	// else...
	return 255; // slotid in eeprom invalid, or eeprom busted
}
