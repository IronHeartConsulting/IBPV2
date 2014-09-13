/*   readFreq
**      exmaple code to read the VFO setting from an CI-V connected ICOM radio
**
*/
#include <BCNDebug.h>
#include <CIV_Commands.h>

#define radio_baudrate 4800


  BCNDebug _dbg;
 
  //Init radio class
  //Assign controller and radio addresses
  const uint8_t controller_address = 0xE0;    //0xE0
  const uint8_t radio_address = 0x76;        //0x76
  CIV radio(controller_address, radio_address,&_dbg);
  unsigned long currentFreq;
  uint8_t buff[128];


  
  void setup()
  {

    delay(5000);  // hold off start up message for 5 seconds
    Serial.println("7200 Read Freq V1.4"); 
    
    Serial1.begin(radio_baudrate);    //default IC7200 is 4800
// Does this belong here or in civ_commands.cpp--CIV?
     _dbg.RS232DebugON();
    //Initialize radio
    initRadio();
    _dbg.RS232DebugOFF();
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
  
  
 
