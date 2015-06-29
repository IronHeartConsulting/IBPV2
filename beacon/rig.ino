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
// ***  _dbg.RS232DebugON();
  radio.set_mode(CW);
  radio.adjustSliders(keyerType,0);  // straight key
  radio.get_ReceiveTransmitfunction(powRF);
  radio.set_ReceiveTransmitfunction(powRF,128);   // set TX power to 50%, or 50 watts
  radio.get_ReceiveTransmitfunction(powRF);  
  newFreq = 14100000;
  radio.set_freq(newFreq);

}

void setband(byte meters) {
  Serial.print(F("Set band to "));
  Serial.print(meters, DEC);
  Serial.println(F("M") );
//  txoff();   // don't even think of QSYing with the RX on
  switch (meters) {
    case 20:
      newFreq = 14100000;
	  FPPRINTRC(1,9,"20");
      break;
    case 17:
      newFreq = 18100000;
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
  Serial.print(F("Set power to ") );
  Serial.print(dBm, DEC);
  Serial.println(F("dBm") );
  switch (dBm) {
    case 50:  // 50 dBm = 100 watts
      newPow = 249;
	  FPPRINTRC(1,12,"50");
      break;
    case 40:  // 40 dBm = 10 watts
	  FPPRINTRC(1,12,"40");
      newPow = 30;
      break;
    case 30:  // 30 dBm = 1 watts
      newPow = 2;
	  FPPRINTRC(1,12,"30");
      break;
    case 20:  // 20 dBm = 100 milliwatts
      newPow = 1;
	  FPPRINTRC(1,12,"20");
      break;
    default:
      newPow = 1;
	  FPPRINTRC(1,12,"20");
      break;
  }
  radio.adjustSliders(rfPower,newPow);
}

// void send_id(char *id) {
//  Serial.print(F("Send CW "));
//  Serial.println(id);
//  id_sent = true;
// }

// Caution: this is called in an interrupt routine!
void txon() {
    if (id_sent) {
    digitalWrite(LED, HIGH);
    digitalWrite(PTTLINE,PTTON);
    FPBLRED
    digitalWrite(CWLINE,HIGH);
    Serial.println(F("TX"));
  } else {
    Serial.println(F("ID Not sent: TX INHIBIT"));
  }
}

void txoff() {
    digitalWrite(LED, LOW);
    digitalWrite(PTTLINE,PTTOFF);
    FPBLGREEN
    digitalWrite(CWLINE,LOW);
    Serial.println(F("RX"));
}
