//
//   Utility to read and write vars into the EEPROM
//
//    Accept commands - and action

#include <SerialCommand.h>
#include <limits.h>
#include <Wire.h>
#include <inttypes.h>
#include <LCDi2cNHD.h>
#include <EEPROM.H>

#include "config.h"
#include "debug.h"
#include "beacon.h"

SerialCommand sCmd;     // The  SerialCommand object

// rows, columns, I2C address, display (not use, set to zero)
LCDi2cNHD fp_lcd = LCDi2cNHD(2,16,0x50>>1,0);

volatile boolean ticked = false;

// wall_ticks counts seconds and goes from 0-180 because there is a 3-minute beacon schedule.
volatile byte wall_ticks = 0;

// Set this to the click second at which you want the tick() interrupt handler to key the transmitter.
volatile byte next_tx_click = 255;

// Set this to true if the tick() interrupt handler to measure update millis_per_second to the most recent value.
// It should never be on if there is a chance that other interrupts will interfere or if interrupts are disabled, such as during SoftwareSerial output (input is OK).
volatile boolean disciplining_milliclock = false;

// The disciplining value for the milliclock, which really means measuring it and recording millis_per_second.
volatile int millis_per_second = 0;

// Outside the interrupt, when disciplining the milliclock, update this with the value needed to achieve 750ms.  It starts at 750 and disciplining the milliclock sets this number.
int seven_fifty_millis = 750;

volatile int last_millis = 0;

// set on id send, reset on band change.  prevents starting up in the middle of the schedule.
volatile boolean id_sent = false;

// tick interrupt
// keep track of wall_ticks; use GPS PPS to discipline millis_per_second when it's safe to do so (no interrupts masked).
void tick() {
  wall_ticks = (wall_ticks+1) % (3*60);
//  if ((wall_ticks - stations[SLOTINDEX].start_time) == next_tx_click) {
//    txon();
//  }

  if (disciplining_milliclock) {
    int t = last_millis;
    last_millis = millis();
    millis_per_second = millis() - t;
  }

  ticked = true;
}

void setup()  {

	// Kill radio TX as soon as we wake up
	pinMode(LED, OUTPUT);	// TX on LED
	pinMode(PTTLINE, OUTPUT);
	pinMode(CWLINE, OUTPUT);
	digitalWrite(PTTLINE, PTTOFF);
	digitalWrite(CWLINE, LOW);

	// FP LCD set up
	pinMode(BLBLUE , OUTPUT);
	pinMode(BLGREEN, OUTPUT);
        pinMode(BLRED,   OUTPUT);   
	
	FPBLBLUE
	fp_lcd.init();
        fp_lcd.cursor_off();
        FPPRINTRC(0,0,"V1.0a     ");
        FPPRINTRC(0,7,"EEPROM Util");
        FPPRINTRC(1,0,"QRX Serial CNSOL");

  // Serial debug output to desktop computer.  For product, send to LCD.
#if DEBUG
	setup_debug_print();
#endif

  debug_println(F("Radio Test V1.0a"));

  FPPRINTRC(1,0,"QRX INIT      ")
  
  // callbacks for various commands expected
sCmd.addCommand("R",	readEEP);	// read a byte from eePROM, and print it
sCmd.addCommand("W",	writeEEP);	// write a byte ot EEPROM

sCmd.setDefaultHandler(wtf);        // handle un-recongized command
    
  FPPRINTRC(1,0,"               ");
  FPPRINTRC(1,0,"CMD Loop");
  FPBLGREEN
	Serial.println("Ready!");

}

void loop()
{

    sCmd.readSerial();

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


