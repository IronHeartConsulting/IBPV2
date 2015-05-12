/*
Arduino CI-V Commands driver
Written by Rocky Purba
March 5th 2011
Version 0.1
*/





#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
  #else
  #include "WProgram.h"
  #endif
#include "BCNDebug.h"
#include "CIV_Commands.h"


/************************************************/
/*	INITIALIZE CLASS							*/
/*	Purpose: set private variables for source	*/
/*			and destination addresses			*/
/************************************************/
CIV::CIV(uint8_t source, uint8_t dest, BCNDebug* dbg)
{
	_src = source;
	_dst = dest;
	SysDebug = dbg;
}


/************************************************/
/*	SEND PREAMBLE								*/
/*	Purpose: indicate radio that data is coming	*/
/************************************************/
void CIV::preAmble()
{
	Serial1.write(0xFE);
	Serial1.write(0xFE);
}



/************************************************/
/*	SEND END of MESSAGE							*/
/*	Purpose: indicate radio end of serial data	*/
/************************************************/
void CIV::EOM()
{
	Serial1.write(0xFD);
}

/************************************************/
/*	SEND N-BYTE DATA							*/
/*	Purpose: send n-bytes to serial interface	*/
/*			maximum payload is 8 bytes			*/
/************************************************/
void CIV::send_nByteData(unsigned long nData, unsigned short width)
{
	uint8_t bcd[8];
	uint8_t pdata[5] = {0x00,0x00,0x00,0x00,0x00};
	int counter = 0;
	//Assuming we have an 8 digit number = 14.250.012
	if(width <= 8)
	{
		for(int i = 0; i < width; i++)
		{	bcd[i] = nData % 10;			//convert number to BCD format
			nData /= 10;					//bcd ={2,1,0,0,5,2,4,1}
			if(i % 2 == 1)					//If odd counter
			{								//Combines 2 bytes into a single byte
											//bcd2 = {12,00,25,14}
				uint8_t bcd2 = (bcd[i] << 4) | bcd[i-1];	//Combines previous bcd with shifted current bcd 
				Serial1.write(bcd2);		//Send bcd data
				pdata[counter] = bcd2;
				counter++;
			}
		}
		SysDebug->PrintDebug(pdata, width, "Sent Packet: \0");

	};
}

/************************************************/
/*	convert2BCDBE  Big Endian                   */
/*	Purpose: send n-bytes to serial interface	*/
/*			maximum payload is 8 bytes			*/
/************************************************/
void CIV::convert2BCDBE(uint8_t *pdata, uint8_t width, unsigned long value)
{
	uint8_t bcd[8];
//	uint8_t pdata[5] = {0x00,0x00,0x00,0x00,0x00};
	int counter = 0;
	//Assuming we have an 8 digit number = 14.250.012
	if(width <= 8)
	{
		for(int i = 0; i < width; i++)
		{	bcd[i] = value % 10;			//convert number to BCD format
			value /= 10;					//bcd ={2,1,0,0,5,2,4,1}
			if(i % 2 == 1)					//If odd counter
			{								//Combines 2 bytes into a single byte
											//bcd2 = {12,00,25,14}
				uint8_t bcd2 = (bcd[i] << 4) | bcd[i-1];	//Combines previous bcd with shifted current bcd 
//				Serial1.write(bcd2);		//Send bcd data
				pdata[counter] = bcd2;
				counter++;
			}
		}
		SysDebug->PrintDebug(pdata, width, "2 BCD BE: \0");

	};
}

/************************************************/
/*	SET MODE TO ALL								*/
/*	Purpose: set all devices attached to the	*/
/*			same mode							*/
/************************************************/
radio_resp CIV::set_mode_transceive(xmit_mode mode)
{
	preAmble();
	Serial1.write((byte)0x00);
	Serial1.write(_src);
	Serial1.write(0x01);
	Serial1.write(mode);
	EOM();
	return read_radio_response();
}

/************************************************/
/*	SET FREQUENCY TO ALL						*/
/*	Purpose: set all devices attached to the	*/
/*			same frequency						*/
/************************************************/
radio_resp CIV::set_freq_transceive(unsigned long freq)
{
	preAmble();
	Serial1.write((byte)0x00);
	Serial1.write(_src);
	Serial1.write((byte)0x00);
	send_nByteData(freq, 8);
	EOM();
	return read_radio_response();
}

/************************************************/
/*	SET MODE									*/
/*	Purpose: set mode of addressed device		*/
/************************************************/
radio_resp CIV::set_mode(xmit_mode mode)
{
	preAmble();
	Serial1.write(_dst);
	Serial1.write(_src);
	Serial1.write(0x06);
	Serial1.write(mode);
	EOM();
	return read_radio_response();
}

/************************************************/
/*	SET FREQUENCY								*/
/*	Purpose: set frequency of addressed device	*/
/*			if succesfull, then it returns true	*/
/*			else it returns false				*/
/************************************************/
radio_resp CIV::set_freq(unsigned long freq)
{
	preAmble();
	Serial1.write(_dst);
	Serial1.write(_src);
	Serial1.write(0x05);
	send_nByteData(freq, 8);
	EOM();
	return read_radio_response();
}



/************************************************/
/*	QUERY FREQUENCY								*/
/*	Purpose: query addressed device for current	*/
/*			frequency setting					*/
/************************************************/
unsigned long CIV::read_freq()
{
	unsigned long freq;
	uint8_t pdata[8];
	preAmble();
	Serial1.write(_dst);
	Serial1.write(_src);
	Serial1.write(0x03);
	EOM();
//	radio_resp response = read_radio_response();
	
	uint8_t length = get_nByteData(pdata,false);
	freq = BCD_Number(pdata,length);
	return freq;
}

/************************************************/
/*	SET RECEIVE TRANSMIT FUNCTION				*/
/*	Purpose: set receive and transmit functions	*/
/************************************************/
radio_resp CIV::set_ReceiveTransmitfunction(subcomm_14 subc, uint8_t value)
{
	preAmble();
	Serial1.write(_dst);
	Serial1.write(_src);
	Serial1.write(0x14);
	Serial1.write(subc);
	Serial1.write(value);
	EOM();
	return read_radio_response();
}


/************************************************/
/*	GET RECEIVE TRANSMIT FUNCTION				*/
/*	Purpose: read data from the 0x14 cmd set    */
/************************************************/
radio_resp CIV::get_ReceiveTransmitfunction(subcomm_14 subc)
{
	preAmble();
	Serial1.write(_dst);
	Serial1.write(_src);
	Serial1.write(0x14);
	Serial1.write(subc);
	EOM();
	return read_radio_response();
}


/************************************************/
/*	TOGGLE FUNCTION								*/
/*	Purpose: turn on or off functions			*/
/************************************************/
radio_resp CIV::set_Togglefunction(subcomm_16 subc, uint8_t value)
{
	preAmble();
	Serial1.write(_dst);
	Serial1.write(_src);
	Serial1.write(0x16);
	Serial1.write(subc);
	Serial1.write(value);
	EOM();
	return read_radio_response();
}


/************************************************/
/*	SET VFO MODE								*/
/*	Purpose: set VFO mode						*/
/************************************************/
radio_resp CIV::set_vfomode(subcomm_07 mode)								//command code 07
{
	preAmble();
	Serial1.write(_dst);
	Serial1.write(_src);
	Serial1.write(0x07);
	Serial1.write(mode);
	EOM();
	return read_radio_response();
}

/************************************************/
/*	TOGGLE SPLIT FUNCTIONALITY					*/
/*	Purpose: turn on or off split functionality	*/
/************************************************/
radio_resp CIV::toggle_Split(subcomm_0F mode)							//command code 0F
{
	preAmble();
	Serial1.write(_dst);
	Serial1.write(_src);
	Serial1.write(0x0F);
	Serial1.write(mode);
	EOM();
	return read_radio_response();
}


/************************************************/
/*	set various sliders and modes 				*/
/*	Purpose: adjust slider value, or change     */
/*  a setting	                                */
/************************************************/
radio_resp CIV::adjustSliders(subcomm_1A slider, uint8_t value)			//command code 1A
{

	uint8_t pdata[8];
	preAmble();
	Serial1.write(_dst);
	Serial1.write(_src);
	Serial1.write(0x1A);
	Serial1.write(0x03);   //note: assume subcmd starts with 03
	Serial1.write(slider);
// convert value to format radio is expecting - by slider type
	switch (slider) {
		case rfPower:
// two bytes - 0 to 255, big endian BCD.  Input is a byte
			convert2BCDBE(&pdata[0], 4, value);
			Serial1.write(pdata[1]);
			Serial1.write(pdata[0]);
			break;
		default:
			Serial1.write(value);
			break;
	}
	EOM();
	return read_radio_response();
}


/************************************************/
/*	READ RESPONSE FROM RADIO					*/
/*	Purpose: read response from radio			*/
/*	                                 			*/
/*	Note: returned data is lost      			*/
/************************************************/
radio_resp	CIV::read_radio_response()
{
	radio_resp bstatus = commandNG;
	uint8_t pdata[MAXBUF];								//pdata will contain response from radio
	uint8_t retsize = get_nByteData(pdata, false);		//read radio response
	if(retsize == 0)
		bstatus = commandTimeout;
	else
	{
		if(pdata[0] == 0xFB)							//check response
			bstatus = commandOK;						//if FB is returned, then radio has succesfully returned a value
		else if(pdata[1] == 0xFA)
			bstatus = commandNG;
		else
			bstatus = commandTimeout;
	};
	
	return bstatus;
	
}

/************************************************/
/*	GET PACKET FROM BUS							*/
/*	Purpose: listen to bus for a packet			*/
/************************************************/
uint8_t CIV::get_SerialData(uint8_t pdata[])
{
	uint8_t buffer_length = 0;
	uint8_t maxtries = 10;
	uint8_t ntries	= 0;						//number of tries
	while(Serial1.peek() == -1 && ntries < maxtries)	//Loop until there is something in serial buffer 
	{
		delay(10);								//delay 10ms
		ntries++;
	};					
	
	//At this point data is available or it has exceeded number of tries
	if(ntries < maxtries)
	{
		do{
			if(Serial1.available())
			{
				pdata[buffer_length] = Serial1.read();
				buffer_length++;
			}
		}while(pdata[buffer_length - 1] != 0xFD);
		
		//debugging stuff
		SysDebug->PrintDebug(pdata, buffer_length, "Received Raw Packet: \0");
		}
	return buffer_length;
}




/************************************************/
/*	PROCESS PACKET								*/
/*	Purpose: process packet and retrieve data	*/
/*			make sure pdata array is large		*/
/*			enough to contain message sent by	*/
/*			radio								*/
/************************************************/
uint8_t CIV::get_nByteData(uint8_t pdata[], boolean bSubCommand)
{
	uint8_t data_length = 0;
	uint8_t start = 5;									//start of data in buffer
	uint8_t buffer[MAXBUF];								//buffer with max of 128 bytes
	uint8_t maxtries = 5;								//Maximum retries to get a packet designated for PC
	//Lets see if a packet designated for PC is available within n-tries
	for(uint8_t i = 0; i < maxtries; i++)
	{
		uint8_t buffer_length = get_SerialData(buffer);	//Read a packet
		uint8_t end = buffer_length - 2;				//endpoint for data in buffer
		if(	buffer_length != 0 &&
			buffer[0] == 0xFE && 
			buffer[1] == 0xFE &&						
			buffer[2] == _src &&						//microcontroller address
			buffer[3] == _dst &&						//radio address
			buffer[buffer_length - 1] == 0xFD)
			{
				if(bSubCommand)
					start = 6;							//if subcommand available, update start of data
														//Extract payload
				for(data_length = 0; data_length <= (end - start); data_length++)
				{
					pdata[data_length] = buffer[data_length + start];
				}
				//debugging stuff
				SysDebug->PrintDebug(pdata, data_length, "Payload data: \0");

				break;									//Done, exit for loop
			}
			else
				delay(100);								//Lets wait for 100 ms before retrying.
	};
	
	
	return data_length;
}


/************************************************/
/*	CONVERT BCD TO NUMBER						*/
/*	Purpose: convert BCD data to number			*/
/************************************************/
unsigned long CIV::BCD_Number(uint8_t pdata[],uint8_t data_length)
{
	unsigned long freqinHz = 0;
	short length = data_length/sizeof(uint8_t);
	for(short i = (length - 1); i > -1; i--)//reverse traversing
	{
		freqinHz = freqinHz * 10;				//shift one decade before performing operation
		freqinHz += (pdata[i] >> 4);			//example 0001 0100 (14 BCD), number = 1
		freqinHz = freqinHz * 10;				//number = 10
		uint8_t lowerNybble = pdata[i] << 4;
		lowerNybble = lowerNybble >> 4;
		freqinHz += lowerNybble;
	}
	return freqinHz;
	
}
