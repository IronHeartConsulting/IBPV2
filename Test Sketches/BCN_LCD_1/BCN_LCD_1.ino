/*      ic7200_test Rev 4 - made from - 1, 2, 3
**        
**      Sample BCN loop
**          set to 100 watts
**          CW ID
**          long dash
**          set to 10 watts
**          long dash
**          sleep for 3 minutes
**
**      set radio to CW, no breakin/QSK, 14.100
**      send callsign with CW.  Delay 30 seconds. loop
**      
**      pin assignments:PC6 / Digitla pin 5 - CW line into radio
**                      PC / Digital pin 12 - PTT output via ACC Connector
**
**       check for power control
*/
#include <BCNDebug.h>
#include <CIV_Commands.h>
#include <morse.h>

#define radio_baudrate 4800
#define CWLINE 5
#define PTTLINE 12
#define PTTON LOW
#define PTTOFF HIGH
#define DELAYTIMER 60  // number of seconds to wait for USB serial monitor to connect


  BCNDebug _dbg;
 
  //Init radio class
  //Assign controller and radio addresses
  const uint8_t controller_address = 0xE0;    //0xE0
  const uint8_t radio_address = 0x76;        //0x76
  CIV radio(controller_address, radio_address,&_dbg);
  unsigned long currentFreq,newFreq;
  uint8_t buff[128];
  LEDMorseSender cwBeacon(CWLINE);
  uint8_t waitCount;


  
  void setup()
  {

    waitCount = 0;   // counter for loop iterations waiting for USB serila monitor to start
    while(!Serial && (waitCount < DELAYTIMER) ) {  // wait for USB serial monitor to become connected.
      delay(1000);   // wait one second
      waitCount++;
    }
    Serial.println("7200 Read Freq V2.1"); 
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
    cwBeacon.setWPM(22.0);
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
// send callsign in CW here
            Serial.println("100 watts");
            
            radio.adjustSliders(rfPower,249);
            radio.get_ReceiveTransmitfunction(powRF);
            
            digitalWrite(PTTLINE,PTTON);
            cwBeacon.setMessage(String("de k6td"));
            cwBeacon.sendBlocking();
            
            digitalWrite(CWLINE,HIGH);    // start long dash
            delay(1000); // key for 1 seconds
            digitalWrite(CWLINE,LOW);    // end long dash

            Serial.println("10 watts"); 
            radio.adjustSliders(rfPower,30);        
            digitalWrite(CWLINE,HIGH);
            delay(1000); // key for 1 seconds
            digitalWrite(CWLINE,LOW);
           
            Serial.println("1 watts"); 
            radio.adjustSliders(rfPower,2);        
            digitalWrite(CWLINE,HIGH);
            delay(1000); // key for 1 seconds
            digitalWrite(CWLINE,LOW);           
            
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
    radio.set_ReceiveTransmitfunction(powRF,128);  // set RF power level - 50%
    radio.get_ReceiveTransmitfunction(powRF);  // read RF power level
    newFreq = 14100000;
    radio.set_freq(newFreq);
    
    //Example
    radio.set_Togglefunction(toggleVOX,0);          //Disable VOX
//    radio.set_ReceiveTransmitfunction(VOX,128);     //Set VOX gain
//    radio.set_ReceiveTransmitfunction(antiVOX,128); //Set anti-VOX gain
   
  }

 
