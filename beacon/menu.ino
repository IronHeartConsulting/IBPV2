#include "beacon.h"
#include "config.h"
#include "BCNDebug.h"
#include <bounce2.h>
#include <LCDi2cNHD.h>


enum button {SHORTPRESS = 1, LONGPRESS, TIMEOUT};
enum button menuBtnPress();

int runMenu() {

	enum button btnPressDuration;
	int cursorPos[3] = {0,2,5};
	int skipValues[3] = {(2*60)/3, (25*60)/3, (49*60)/3};
	int selectionIndex;

	btnPressDuration = menuBtnPress();
	if (btnPressDuration == TIMEOUT)
		return(0);
// menu button pressed.  Either go to serial Console, or menu on LCD 
	if( Serial) {  // serial UART active - connect thru it
		runConsole();
		return(0);
	}
	fp_lcd.clear();
	FPBLGREEN
// show menu
	FPPRINTRC(0,0,F("SKIP - HOURS Off"));
	FPPRINTRC(1,0,F("2 25 49"));
	fp_lcd.blink_on();
	fp_lcd.setCursor(1,0); // blinking cursor on first skip selection
	menuBtn.update();
	// spin till the menu button isn't pressed, or we time out
	while(menuBtn.read() == HIGH) 
		menuBtn.update();
	// now we are into processing the menu
	selectionIndex = 0;
	while(1)  {  // a return gets us out
		btnPressDuration = menuBtnPress();
		switch (btnPressDuration) {
		  case SHORTPRESS:  // move cursor forward
			selectionIndex = (selectionIndex + 1) % 3;
			fp_lcd.setCursor(1,cursorPos[selectionIndex]);
			break;
		  case LONGPRESS:
			remainingSkipCount = skipValues[selectionIndex];
			skipEnabled = 1;
			FPPRINTRC(0,0,F("SKIP Enabled      "));
			FPPRINTRC(1,0,F("For:"));
			FPPRINTRC(1,5,remainingSkipCount);
			FPPRINTRC(1,9,F("cycles"));
			delay(5*1000);  // let the op see the selection
			return(0);
			break;
		  case TIMEOUT:
			return(0);   // don't stall if the op stops using the menu
			break;
		}
	}

	return(0);

}

// look for a press of the menu button.
//  use duraiton of press to return long or short press
enum button menuBtnPress()  {
	uint32_t holdCount = 0;
	long chkTimer;  // time entered a spin loop

	menuBtn.update();
	chkTimer = millis();
	// spin till the menu button isn't pressed, or we time out
	while(menuBtn.read() == HIGH)  {
		menuBtn.update();
		if ((millis() - chkTimer) > (10*1000) )
			return(TIMEOUT);
	}
	// ignore button not pressed
	chkTimer = millis();
	while(menuBtn.read() == LOW )  {
		menuBtn.update();
		// time out check
		if ((millis() - chkTimer) > (10*1000) )
			return(TIMEOUT);
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
