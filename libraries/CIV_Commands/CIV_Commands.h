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

#define MAXBUF 128

enum radio_resp{commandNG = 0, commandOK = 1, commandTimeout = 2};
enum subcomm_07{vfoA = 0x00, vfoB = 0x01, equalizeAB = 0xA0, exchangeAB = 0xB0};

enum subcomm_0F{splitOFF = 0x00, splitON = 0x01};

enum subcomm_14{AF = 0x01, RF = 0x02, SQL = 0x03, NR = 0x06, inPBT = 0x07,
                outPBT = 0x08, CWpitch = 0x09, powRF = 0x0A, MIC = 0x0B, KEY = 0x0C,
                MNF = 0x0D, COMP = 0x0E, BREAKin = 0x0F, NB = 0x12, VOX =0x16, antiVOX =0x17};

enum subcomm_16{togglePreamp = 0x02,AGC = 0x12, toggleNoiseBlanker = 0x22, toggleNoiseRedux = 0x40,
                toggleAutoNotch = 0x41, toggleSpeechComp = 0x44, toggleVOX = 0x46,
                toggleBREAKin = 0x47, toggleManualNotch = 0x48, toggleTwinPeak = 0x4F, toggleDialLock = 0x50};

enum xmit_mode{LSB = 0, USB = 1, AM = 2, CW = 3, RTTY = 4, FM = 5};

class CIV
{
	public:
      CIV(uint8_t source, uint8_t dest, BCNDebug dbg);
	  
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
		unsigned long BCD_Number(uint8_t pdata[], uint8_t data_length);
		uint8_t _src;															//variable to hold source address - controller
		uint8_t _dst;															//variable to hold destination address - radio
		BCNDebug SysDebug;
};
#endif

