/*
Arduino CI-V Commands driver
Written by Rocky Purba
March 5th 2011
Version 0.1
*/

#ifndef CIV_h
#define CIV_h

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
  #else
  #include "WProgram.h"
  #endif

#include "..\..\libraries\Beacon_Types\Beacon_Types.h"

class CIV
{
	public:
      CIV(uint8_t source, uint8_t dest, cDebug *dbgFlag);
	  
      radio_resp set_freq_transceive(unsigned long freq);						//command code 00
	  radio_resp set_mode_transceive(xmit_mode mode);							//command code 01
	  unsigned long read_freq();												//command code 03
	  radio_resp set_freq(unsigned long freq);									//command code 05
	  radio_resp set_mode(xmit_mode mode);										//command code 06
	  radio_resp set_vfomode(subcomm_07 mode);									//command code 07
	  radio_resp set_ReceiveTransmitfunction(subcomm_14 subc, uint8_t value);	//command code 14
	  radio_resp set_Togglefunction(subcomm_16 subc, uint8_t value);			//command code 16
	  radio_resp toggle_Split(subcomm_0F mode);									//command code 0F
	private:
		void preAmble();														//send FEFE
		void EOM();																//send FD
		void send_nByteData(unsigned long nData, unsigned short width);
		radio_resp	read_radio_response();
		uint8_t get_SerialData(uint8_t pdata[]);
		uint8_t get_nByteData(uint8_t pdata[], boolean bSubCommand);
		unsigned long BCD_Number(uint8_t pdata[]);
		void PrintDebug(uint8_t *pdata, uint8_t data_length, char *message);
		uint8_t _src;															//variable to hold source address - controller
		uint8_t _dst;															//variable to hold destination address - radio
		cDebug *_debug;															//variable to hold debug object
};
#endif