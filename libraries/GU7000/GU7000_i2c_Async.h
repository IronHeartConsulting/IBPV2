#ifndef GU7000_I2C_ASYNC_H
#define GU7000_I2C_ASYNC_H

#include <Noritake_VFD_GU7000.h>
#include <avr/pgmspace.h>
#include <Wire.h>

//
// interface class for GU7000 VFD
//    I2C communication to a UART (NXP SC16IS750)
//

//
// F_CPU ==  16000000
//

//template <unsigned NORITAKE_VFD_BAUD>
class GU7000_I2C_Async : public GU7000_Interface {

private:
	Noritake_VFD_GU7000 GU7000;

protected:

// Need to assign SCL and SDA Pins
	uint16_t vfd_baudrate;
	uint8_t addrUART; // I2C address of UART
    
public:
	
    GU7000_I2C_Async(long baudRate, uint8_t addrUART ):
		baudRate = 38400, addrUART = 0x48;
    {

		
	// init I2C interface
	Wire.begin();  // assume standard I2C interface for Leonardo

	}
	
	uint8_t writeUART(uint8_t addrReg, uint8_t data) {
		uint16_t i2cResult;
		Wire.beginTransmission(addrUART);
		Wire.write(addrReg);
		Wire.write(data);
		i2cResult = Wire.endTransmission();
	}

	uint8_t readUART(uint8_t addrReg, uint8_t data[], uint8_t count) {
		uint16_t i2cResult;
		Wire.beginTransmission(addrUART);
		Wire.write(addrReg);
		i2cResult = Wire.endTransmission();
		if (i2cResult != 0 ) 
			return(-i2cResult);
		bytesRead = Wire.requestFrom(addrUART, count, true);
		for (i = 0; i < byteRead, i++ ) {
			data[i] = Wire.read();
		}
		return bytesRead;
	}

    void init() {

// This is all poking the UART over I2C for setup

		writeUART(IOCtl,0x08);   // reset the UART chip
		do {
			delay(1);  // delay 1 ms - don't know how long UART takes to reset
			readUART(IOCtl, UARTData[], 1);  // poll till bit clears
		} while (UARTData[0] & 0x08);   
		// set the baud rate
		writeUART(LCR, 0x80);  // turn on access to BRG regs
		writeUART(DLH, 0);
		writeUART(DLL, 6);		// baud rate = 38400 = H.C.
		// enable auto-CTS
		writeUART(LCR, 0xbf);  // access to EFR reg
		writeUART(EFR, 0x80);
		// set LCR for normal reg access and 1 stop bt, word len 8 bits, no parity
		writeUART(LCR,0x03);

		// set GPIO pin directions
		writeUART(IODir, 0x01);
		writeUART(FCR, 0x07);  // clear FIFOs, enable TX FIFO
		writeUART(EFCR, 0x02); // disable RX - VFD doesn't have any thing to say

// UART configed
		writeUART(IOState,0x01);  // clear VFD reset line

    }

    void write(uint8_t data) {
		// Check for busy signal
		
		
		if (!_inverse_logic)
			while (CHECK(BUSY));
		else
			while (!CHECK(BUSY));
		
		uint8_t oldSREG = SREG;
		cli();  // turn off interrupts for a clean txmit

		// Write the start bit
		tx_pin_write(_inverse_logic ? HIGH : LOW);
		tunedDelay(_tx_delay + XMIT_START_ADJUSTMENT);
		
		// Write each of the 8 bits
		if (!_inverse_logic)
		{
			for (byte i = 0x01; i; i <<= 1) {
				
				if (data & i)
    				tx_pin_write(HIGH); // send 0
				else
					tx_pin_write(LOW); // send 1
				tunedDelay(_tx_delay);
    			
    		}
			tx_pin_write(HIGH);
		}
		else
		{
			for (byte i = 0x01; i; i <<= 1) {
				if (data & i)
    				tx_pin_write(LOW); // send 1
				else
					tx_pin_write(HIGH); // send 0
				tunedDelay(_tx_delay);
    		}
			tx_pin_write(LOW);
		}
		SREG = oldSREG; // turn interrupts back on
		tunedDelay(_tx_delay);
	}

   
    void hardReset() {
        init();
		writeUART(IOState,0x00);  // set VFD reset line
    	_delay_ms(1);
		writeUART(IOState,0x01);  // clear VFD reset line
    	_delay_ms(100);
		
	}
};

#endif
