/*      ic7200_test Rev 2 - made from _1
**        readFreq
**      exmaple code to read the VFO setting from an CI-V connected ICOM radio
**
**      set radio to CW, no breakin/QSK, 14.100
**      send callsign with CW.  Delay 30 seconds. loop
**      
**      pin assignments:PC6 / Digitla pin 5 - CW line into radio
**                      PC / Digital pin 13 - PTT output via ACC Connector
**
**       check for power control
*/
#include <BCNDebug.h>
#include <CIV_Commands.h>
#include <morse.h>

#define radio_baudrate 4800
#define CWLINE 5
#define PTTLINE 13
#define PTTON LOW
#define PTTOFF HIGH


  BCNDebug _dbg;
 
  //Init radio class
  //Assign controller and radio addresses
  const uint8_t controller_address = 0xE0;    //0xE0
  const uint8_t radio_address = 0x76;        //0x76
  CIV radio(controller_address, radio_address,&_dbg);
  unsigned long currentFreq,newFreq;
  uint8_t buff[128];
  LEDMorseSender cwBeacon(CWLINE);


  
  void setup()
  {

    delay(5000);  // hold off start up message for 5 seconds
    Serial.println("7200 Read Freq V1.5"); 
    digitalWrite(PTTLINE,PTTOFF);
    pinMode(CWLINE,OUTPUT);
    pinMode(PTTLINE,OUTPUT);
    Serial1.begin(radio_baudrate);    //default IC7200 is 4800
// Does this belong here or in civ_commands.cpp--CIV?
     _dbg.RS232DebugON();
    //Initialize radio
    initRadio();
    _dbg.RS232DebugOFF();
// Set up CW
    cwBeacon.setup();
    cwBeacon.setWPM(22);
  }
  
  void loop()
  {
            unsigned long currentFreq;
            int freqMHzPortion, freqKhzPortion, freqHzPortion;
            currentFreq = radio.read_freq();
            freqMHzPortion = currentFreq/10e5;
            freqHzPortion = (currentFreq/10e2);  // mask out lower three digits
            freqHzPortion *= 10e2;
            freqHzPortion = currentFreq - freqHzPortion;   // clear only the lower three digits
            freqKhzPortion = currentFreq/10e2;
            freqKhzPortion = freqKhzPortion - (freqMHzPortion*10e2);
            Serial.print(freqMHzPortion);
            Serial.print(".");
            Serial.print(freqKhzPortion);
            Serial.print(".");
            Serial.print(freqHzPortion);
            Serial.println(" Hz");
            Serial.println("starting BCN CW");
            digitalWrite(PTTLINE,PTTON);
// send callsign in CW here
            cwBeacon.setMessage(String("de k6td"));
            cwBeacon.sendBlocking();
            delay(5000); // key for 5 seconds
            digitalWrite(PTTLINE,PTTOFF);
            Serial.println("BCN off, waiting for next time slot");
            delay(30000);
  }
  
  /*
  INITIALIZE RADIO
  PURPOSE: Perform initialization procedure for radio 
  */
  void initRadio()
  {

    
    //Set variable for transmit mode to CW
    radio.set_mode(CW);
    radio.adjustSliders(keyerType,0);  // set keyer to straight
    radio.get_ReceiveTransmitfunction(powRF);  // read RF power level
//    radio.adjustSliders(rfPower,255);  // set RF power to 50%
    newFreq = 14100000;
    radio.set_freq(newFreq);
    
    //Example
    radio.set_Togglefunction(toggleVOX,1);          //Enable VOX
    radio.set_ReceiveTransmitfunction(VOX,128);     //Set VOX gain
    radio.set_ReceiveTransmitfunction(antiVOX,128); //Set anti-VOX gain
   
  }

 
