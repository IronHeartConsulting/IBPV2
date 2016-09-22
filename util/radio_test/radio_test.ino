//
// **** radio_test.ino ****
//
//   Utility to control radio to meausre power output
//
//    Accept commands - and action
//		b - programs radio to one of the beacon frequencies.
//			input: 20m | 17m | 15m | 12m | 10m
//		p - sets transmit power level
//			input: decimal number between 255 and 0, inclusive. Value is programmed into CI-V command XXXX
//		t - turns TX on or off
//			input: ON | OFF - note: program will turn TX off after 30 seocnds on
//		A - control ALC Power inverter

#include <SerialCommand.h>
#include <limits.h>
#include <Wire.h>
#include <inttypes.h>
#include <LCDi2cNHD.h>

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
        setALCPwr(LOW);
	pinMode(PTTLINE, OUTPUT);
	txoff();

	// FP LCD set up
	pinMode(BLBLUE , OUTPUT);
	pinMode(BLGREEN, OUTPUT);
        pinMode(BLRED,   OUTPUT);   
	
	FPBLBLUE
	fp_lcd.init();
        fp_lcd.cursor_off();
        FPPRINTRC(0,0,"V1.0b     ");
        FPPRINTRC(0,7,"Radio Test");
        FPPRINTRC(1,0,"QRX Serial CNSOL");

  // Serial debug output to desktop computer.  For product, send to LCD.
#if DEBUG
	setup_debug_print();
#endif

  debug_println(F("Radio Test V1.0b"));

  FPPRINTRC(1,0,"QRX INIT      ")
  
  // callbacks for various commands expected
sCmd.addCommand("TX",  radioON);    // Key down
sCmd.addCommand("RX",  radioOFF);   // Key up
sCmd.addCommand("B",   bandSet);    // QSY to the beacond frequaency spec'ed (by band)
sCmd.addCommand("P",   pwrSet);     // set the TX power level 0-255 value plugged into the CI-V/7200 command adjustsliders subcomm
sCmd.addCommand("P2",   pwrSetM2);     // set power using the set transmit command
sCmd.addCommand("ALC",   alcCtl);     // turn ALC power on or off

sCmd.setDefaultHandler(wtf);        // handle un-recongized command
    
  FPPRINTRC(1,0,"QRX INIT RADIO")
  debug_println(F("Radio init"));
  radioSetup();
  
  FPPRINTRC(1,0,"               ");
  FPPRINTRC(1,0,"CMD Loop");
  FPBLGREEN

}

void loop()
{

    sCmd.readSerial();

}

void radioON() {
  // timeout after 2 minutes
  Serial.println("Key down");
  id_sent = 1;
  txon();
  
}

void radioOFF() {
  Serial.println("Key up");
  txoff();
}

// set the radio to the beacon frequency for the specificed band (in wavelength - 20m, 17m, 15m, 12m, 10m)
void bandSet() {
  char *arg;
  unsigned int bandValue;
  
    arg = sCmd.next();
    if (arg != NULL) { // process the band selection
      bandValue = atoi(arg);
      Serial.print("Band selected:");
      Serial.print(bandValue);
      Serial.println("m");
      setband(bandValue);
    }
    else {
      Serial.println("No band specified");
    }  
}

void pwrSet() {
    char *arg;
  unsigned int pwrValue;
  
    arg = sCmd.next();
    if (arg != NULL) { // process the band selection
      pwrValue = atoi(arg);
      Serial.print("TX Power Value:");
      Serial.print(pwrValue);
      Serial.println("");
      setRawPwr(pwrValue);
    }
    else {
      Serial.println("No power level specified");
    }  
}

void pwrSetM2() {
    char *arg;
  unsigned int pwrValue;
  
    arg = sCmd.next();
    if (arg != NULL) { // process the band selection
      pwrValue = atoi(arg);
      Serial.print("TX Power Value:");
      Serial.print(pwrValue);
      Serial.println(" (M2)");
      setRawPwrM2(pwrValue);
    }
    else {
      Serial.println("No power level specified");
    }  
}

void alcCtl() {
    char *arg;
  
    arg = sCmd.next();
    if (arg != NULL) { // process the band selection
      Serial.print("ALC setting:");
      Serial.print(arg);
		if (strncmp(arg,"ON",2) == 0 )
			setALCPwr(HIGH);
		else if (strncmp(arg,"OFF",2) == 0)
			setALCPwr(LOW);
                else
                  Serial.print("unknown ALC setting");
      Serial.println(" ");
    }
    else {
      Serial.println("No ALC setting specified");
    }  
}

// This gets set as the default handler, and gets called when no other command matches.
void wtf(const char *command) {
  Serial.println("What?");
}


