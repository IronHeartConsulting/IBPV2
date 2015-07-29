/*

  more interesting and identifiable blink pattern
  ************
  
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */
#define lOFF digitalWrite(13,LOW);
#define lON digitalWrite(13,HIGH);

// the setup function runs once when you press reset or power the board
void setup() {
  uint8_t i;
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  
  lOFF
  // setup loop blink rate...fast on, mostly off  
  for (i=0; i<10; i++) {
    lON
    delay(50);
    lOFF
    delay(500);
  }
  delay(1850);
  
}

// the loop function runs over and over again forever
void loop() {
  lON
  delay(100); 
  lOFF
  delay(100);
  lON
  delay(500);
  lOFF
  delay(1000);              // wait for a second
}
