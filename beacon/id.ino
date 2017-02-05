// DIT = 1200 / WPM
// DIT: DIT duration in milliseconds
// WPM: speed in WPM
// WPM=22, DIT=54.55ms, DAH=163.64ms

#include "beacon.h"
#include "morse.h"

LEDMorseSender cwBeacon(CWLINE);

void CWSetup(char *id) {
  cwBeacon.setup();
  cwBeacon.setWPM(22.0);
  cwBeacon.setMessage(id);
}  

void send_id() {
  debug_print(F("send CW id:"));
  debug_println(id);
  digitalWrite(CWLINE,LOW);
  digitalWrite(PTTLINE,PTTON);
//   digitalWrite(LED, HIGH);
//***  FPBLRED
  cwBeacon.sendBlocking();
  id_sent = true;
  digitalWrite(CWLINE,LOW);
//   digitalWrite(LED, LOW);
}

void longDAH() {
  digitalWrite(CWLINE,HIGH);
}
