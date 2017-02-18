#include <SerialCommand.h>
#include <limits.h>
#include <Wire.h>
#include <inttypes.h>
#include "beacon.h"
#include "config.h"
#include "BCNDebug.h"
#include <LCDi2cNHD.h>
#include <EEPROM.H>
SerialCommand sCmd;     // The  SerialCommand object

int runConsole()   {

  // callbacks for various commands expected
	sCmd.addCommand("R",	readEEP);	// read a byte from eePROM, and print it
	sCmd.addCommand("W",	writeEEP);	// write a byte ot EEPROM

	sCmd.setDefaultHandler(wtf);        // handle un-recongized command

	fp_lcd.clear();
	FPBLGREEN
	FPPRINTRC(0,0,F("CONS connected"));
	while(1) {
		Serial.println("Ready!");
    	sCmd.readSerial();
	}
	return(0);


}

// reed a byte from the EEPROM and print it
void readEEP() {
  char *arg;
  unsigned int EEPoffset;
	byte EEPvalue;
  
    arg = sCmd.next();
    if (arg != NULL) { // process the band selection
		EEPoffset = atoi(arg);
		EEPvalue = EEPROM.read(EEPoffset);
		Serial.print("R:");
		Serial.print(EEPoffset);
		Serial.print(":");
		Serial.println(EEPvalue,HEX);
    }
    else {
      Serial.println("No read offset address supplied, try again");
    }  
}

// write a byte ot EEPROM
void writeEEP() {
  char *arg;
  unsigned int EEPoffset;
	byte EEPvalue;
  
    arg = sCmd.next();
    if (arg != NULL) { // get the offset address to write
      EEPoffset = atoi(arg);
    }
    else {
      Serial.println("not enough info: W <offset addr> <value>");
		return;
	}

    arg = sCmd.next();
    if (arg != NULL) { // get the byte value to write
      EEPvalue = atoi(arg);
    }
    else {
      Serial.println("not enough info: W <offset addr> <value>");
		return;
    }  
	EEPROM.write(EEPoffset, EEPvalue);
	Serial.print("W:");
    Serial.print(EEPoffset);
    Serial.print(":");
    Serial.println(EEPvalue,HEX);
}
// This gets set as the default handler, and gets called when no other command matches.
void wtf(const char *command) {
  Serial.println("What?");
}


