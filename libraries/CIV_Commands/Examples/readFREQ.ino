#include "Beacon_Types.h"
#include "CIV_Commands.h"

  //Debug
  cDebug dbg(dbgRS232_ON);
  //Init radio class
  //Assign controller and radio addresses
  const uint8_t controller_address = 0xE0;    //0xE0
  const uint8_t radio_address = 0x76;        //0x76
  CIV radio(controller_address, radio_address, &dbg);
  unsigned long currentFreq;


  
  void setup()
  {
    Serial1.begin(radio_baudrate);    //default IC7200 is 4800
// Does this belong here or in civ_commands.cpp--CIV

    uint8_t buff[128];
    
    //Initialize radio
    initRadio();
    
  }
  
  void loop()
  {
            unsigned long currentFreq;
            currentFreq = radio.read_freq();
            Serial.println(currentFreq);
            delay(2000);
  }
  
  /*
  INITIALIZE RADIO
  PURPOSE: Perform initialization procedure for radio 
  */
  void initRadio()
  {

    
    //Set variable for transmit mode to CW
    radio.set_mode(CW);
    
    //Example
    radio.set_Togglefunction(toggleVOX,1);          //Enable VOX
    radio.set_ReceiveTransmitfunction(VOX,128);     //Set VOX gain
    radio.set_ReceiveTransmitfunction(antiVOX,128); //Set anti-VOX gain
    
  }
  
  
 
