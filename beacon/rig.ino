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
  txoff();   // don't even think of QSYing with the RX on
  switch (meters) {
    case 20:
      newFreq = 1410000;
      break;
    case 17:
      newFreq = 1810000;
      break;
    case 15:
      newFreq = 2115000;
      break;
    case 12:
      newFreq = 2493000;
      break;
    case 10:
      newFreq = 2820000;
      break;
    default:
      newFreq = 2820000;
      break;
  }
  radio.set_freq(newFreq);  
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
      break;
    case 40:  // 40 dBm = 10 watts
      newPow = 30;
      break;
    case 30:  // 30 dBm = 1 watts
      newPow = 2;
      break;
    default:
      newPow = 2;
      break;
  }
  radio.adjustSliders(rfPower,newPow);
}


void txon() {
    digitalWrite(LED, HIGH);
    digitalWrite(PTTLINE,PTTON);
    Serial.println(F("TX"));
}

void txoff() {
    digitalWrite(LED, LOW);
    digitalWrite(PTTLINE,PTTOFF);
    Serial.println(F("RX"));
}
