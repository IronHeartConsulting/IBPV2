#include "beacon.h"

void setband(byte meters) {
  Serial.print(F("Set band to "));
  Serial.print(meters, DEC);
  Serial.println(F("M") );
}

void setpower(byte dBm) {
  // 100W = 50dBm
  // 10W = 40dBm
  // 1W = 30dBm
  // 100mW = 20dBm
  Serial.print(F("Set power to ") );
  Serial.print(dBm, DEC);
  Serial.println(F("dBm") );
}

void send_id(char *id) {
  Serial.print(F("Send CW "));
  Serial.println(id);
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
