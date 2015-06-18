// DIT = 1200 / WPM
// DIT: DIT duration in milliseconds
// WPM: speed in WPM
// WPM=22, DIT=54.55ms, DAH=163.64ms

#include "beacon.h"
#include "morse.h"

LEDMorseSender cwBeacon(CWLINE);

void CWSetup() {
  cwBeacon.setup();
  cwBeacon.setWPM(22.0);
}  

void send_id(char *id) {
  Serial.print(F("send CW id:"));
  Serial.println(id);
  digitalWrite(CWLINE,LOW);
  digitalWrite(PTTLINE,PTTON);
  FPBLRED
  cwBeacon.setMessage(id);
  cwBeacon.sendBlocking();
  id_sent = true;
}

void longDAH() {
  digitalWrite(CWLINE,HIGH);
}
