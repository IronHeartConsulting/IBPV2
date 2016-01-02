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
  Serial1.begin(radio_baudrate);
  
// init the radio
#if DEBUG
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
	  FPPRINTRC(1,9,"20");
      break;
    case 17:
      newFreq = 18110000;
	  FPPRINTRC(1,9,"17");
      break;
    case 15:
      newFreq = 21150000;
	  FPPRINTRC(1,9,"15");
      break;
    case 12:
      newFreq = 24930000;
	  FPPRINTRC(1,9,"12");
      break;
    case 10:
      newFreq = 28200000;
	  FPPRINTRC(1,9,"10");
      break;
    default:
      newFreq = 28200000;
	  FPPRINTRC(1,9,"ER");
      break;
  }
  radio.set_freq(newFreq);  
  id_sent = false;
}

void setpower(byte dBm) {
  // 100W = 50dBm
  // 10W = 40dBm
  // 1W = 30dBm
  // 100mW = 20dBm
  uint16_t newPow;
  FPPRINTRC(1,12,dBm);
  debug_print(F("Set power to ") );
  debug_print_dec(dBm);
  debug_println(F("dBm") );
  switch (dBm) {
    case 50:  // 50 dBm = 100 watts
      newPow = 249;
      break;
    case 40:  // 40 dBm = 10 watts
      newPow = 30;
      break;
    case 30:  // 30 dBm = 1 watts
      newPow = 2;
      break;
    case 20:  // 20 dBm = 100 milliwatts
      newPow = 1;
      break;
    default:
      newPow = 1;
      break;
  }
  radio.adjustSliders(rfPower,newPow);
}

// Caution: this is called in an interrupt routine!
void txon() {
    if (id_sent) {
    digitalWrite(LED, HIGH);
    digitalWrite(PTTLINE,PTTON);
    FPBLRED
    digitalWrite(CWLINE,HIGH);
    debug_println(F("TX"));
  } else {
    debug_println(F("ID Not sent: TX INHIBIT"));
  }
}

void txoff() {
    digitalWrite(LED, LOW);
    digitalWrite(PTTLINE,PTTOFF);
    FPBLGREEN
    digitalWrite(CWLINE,LOW);
    debug_println(F("RX"));
}
