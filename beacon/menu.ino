#include "beacon.h"
#include "config.h"
#include "BCNDebug.h"
#include <bounce2.h>
#include <LCDi2cNHD.h>


enum button {SHORTPRESS = 1, LONGPRESS, TIMEOUT};
enum button menuBtnPress();

int runMenu() {

	enum button btnPressDuration;

	if (menuBtn.read() == LOW ) {
		FPBLBLUE
		return(0);
	}
	// menu button pressed.  Display the menu
	fp_lcd.clear();
	FPBLGREEN
	FPPRINTRC(0,0,F("TIMESLOT GPS"));
	FPPRINTRC(1,0,F("NEXT SEL UP"));
	menuBtn.update();
	// spin till the menu button isn't pressed, or we time out
	while(menuBtn.read() == HIGH) 
		menuBtn.update();
	// now we are into processing the menu
	btnPressDuration = menuBtnPress();
	if (btnPressDuration == SHORTPRESS) {
		debug_println(F("short press"));
		debug_println(F("blink timeslot"));
		FPPRINTRC(0,0,F("TIMESLOT GPS"));
		fp_lcd.blink_on();
		fp_lcd.setCursor(0,0);
//		fp_lcd.blink_off();
		btnPressDuration = menuBtnPress();  // cursor on timeslot
//  next press is short - advance cursor; long - select timeslot
		if (btnPressDuration == SHORTPRESS) {
			debug_println(F("advance"));
			fp_lcd.setCursor(0,10);
		}
		else {  // long press - time slot assignment selected
			debug_println(F("assign timeslot"));
			byte slotid;
			for (slotid = 0; slotid <= MAX_SLOTID; slotid++) {
				debug_println(slotid);
				FPPRINTRC(0,0,F("              "));
				FPPRINTRC(0,0,stations[slotid].call);
				FPPRINTRC(0,8,stations[slotid].start_time);
				fp_lcd.setCursor(1,0);
				// displayed a station callsign
				// wait for UI - next, select, up
				btnPressDuration = menuBtnPress();
				if (btnPressDuration == SHORTPRESS)  // next
					continue;
				// else LONGPRESS fall thru 'continue' above *******
				fp_lcd.setCursor(1,5);
				// LONGPRESS - don't advance till button released
				while(menuBtn.read() == HIGH) 
					menuBtn.update();
				// looking for a long press to confirm SLOTID selected
				btnPressDuration = menuBtnPress();
				if (btnPressDuration == LONGPRESS) { // slot id sleetion confirmed
					// write the SLOTID to eeprom
					set_eeprom_slotid(slotid);
					debug_println(F("timeslot programmed"));
					break; // stop this loop
				}
				else { // short press, go back to top menu
				}
			}		
		}
	}
	else {
		debug_println(F("long press"));
	}


	return(0);

}

// look for a press of the menu button.
//  use duraiton of press to return long or short press
enum button menuBtnPress()  {
	uint32_t holdCount = 0;

	menuBtn.update();
	// spin till the menu button isn't pressed, or we time out
	while(menuBtn.read() == HIGH) 
		menuBtn.update();
	// ignore button not pressed
	while(menuBtn.read() == LOW )  {
		menuBtn.update();
		// time out check
	}
	// button went high / pressed
	while(menuBtn.read() == HIGH )  {
		menuBtn.update();
		holdCount++;
		if (holdCount > 70000) 
			break;  // timeout - LONGPRESS
	}
	if (holdCount < 20000) 
		return SHORTPRESS;
	else
		return LONGPRESS;
	debug_print(holdCount);
	debug_println(F("  "));

}
