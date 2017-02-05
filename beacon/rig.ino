#include "beacon.h"
#include "BCNDebug.h"
#include "CIV_Commands.h"

// constants used by CI-V interface
const uint8_t controller_address = CIV_CA;  // our CI-V address
const uint8_t radio_address = CIV_RA;         // radio's address
uint32_t currentFreq, newFreq;
uint8_t buff[MAXBUF];
BCNDebug _dbg;
CIV radio(controller_address, radio_address, &_dbg);


// init the radio and the CI-V interface
void radioSetup() {

  digitalWrite(PTTLINE, PTTOFF);
  pinMode(CWLINE, OUTPUT);
  pinMode(PTTLINE, OUTPUT);
  setALCPwr(LOW);
  Serial1.begin(radio_baudrate);
  
// init the radio
#if DEBUG_RADIO
	_dbg.RS232DebugON();
#endif

  radio.set_mode(CW);
  radio.adjustSliders(keyerType,0);  // straight key
  radio.get_ReceiveTransmitfunction(powRF);
  radio.set_ReceiveTransmitfunction(powRF,128);   // set TX power to 50%, or 50 watts
  radio.get_ReceiveTransmitfunction(powRF);  
  newFreq = 14100000;
  radio.set_freq(newFreq);

}

void setband(byte meters) {
  debug_print(F("Set band to "));
  debug_print_dec(meters);
  debug_println(F("M") );
//  txoff();   // don't even think of QSYing with the RX on
  switch (meters) {
    case 20:
      newFreq = 14100000;
	  FPPRINTRC(1,7,"20m");
      break;
    case 17:
      newFreq = 18110000;
	  FPPRINTRC(1,7,"17m");
      break;
    case 15:
      newFreq = 21150000;
	  FPPRINTRC(1,7,"15m");
      break;
    case 12:
      newFreq = 24930000;
	  FPPRINTRC(1,7,"12m");
      break;
    case 10:
      newFreq = 28200000;
	  FPPRINTRC(1,7,"10m");
      break;
    default:
      newFreq = 28200000;
	  FPPRINTRC(1,7,"ERR");
      break;
  }
  radio.set_freq(newFreq);  
  id_sent = false;
}

void setRadioMode(rMode Mode) {
  
}

void setpower(byte dBm) {
  // 100W = 50dBm
  // 10W = 40dBm
  // 1W = 30dBm
  // 100mW = 20dBm
  uint16_t newPow;
  FPPRINTRC(1,11,dBm);
  FPPRINTRC(1,13,"dBm");
  debug_print(F("Set power to ") );
  debug_print_dec(dBm);
  debug_println(F("dBm") );
  switch (dBm) {
    case 50:  // 50 dBm = 100 watts
      newPow = 230;
      break;
    case 40:  // 40 dBm = 10 watts
      newPow = 23;
      break;
    case 30:  // 30 dBm = 1 watts
      newPow = 0;
      break;
    case 20:  // 20 dBm = 100 milliwatts
//	turn on the ALC voltage.  We do it here to give it time to stablizie before key down
//***		digitalWrite(ALC_PWR, HIGH);
      newPow = 0;
      break;
    default:
      newPow = 0;
      break;
  }
  radio.adjustSliders(rfPower,newPow);
}

void setRawPwr(byte newPow) {
    radio.adjustSliders(rfPower,newPow);
}

//  set the power level using the 0x14 comand set
void setRawPwrM2(byte newPow) {
	radio.set_ReceiveTransmitfunction(powRF,newPow);   
}

void setALCPwr(byte newState) {
		digitalWrite(ALC_PWR, newState);
	
}

// Caution: this is called in an interrupt routine!
void txon() {
    if (id_sent) {
//  USER LED PIN now turns on the ALC OUTPUT power.  Well, it also still lights the user LED
// **	    digitalWrite(LED, HIGH);
   	 digitalWrite(PTTLINE,PTTON);
   	 FPBLRED
   	 digitalWrite(CWLINE,HIGH);
   	 debug_println(F("TX"));
  } else {
    debug_println(F("ID Not sent: TX INHIBIT"));
  }
}

void txoff() {
	digitalWrite(ALC_PWR, LOW);
// ***    digitalWrite(LED, LOW);
    digitalWrite(PTTLINE,PTTOFF);
	setALCPwr(LOW);
    FPBLGREEN
    FPPRINTRC(1,0,"OPER");
    digitalWrite(CWLINE,LOW);
    debug_println(F("RX"));
}
