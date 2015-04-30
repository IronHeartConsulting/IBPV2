#ifndef GU7000_SERIAL_ASYNC_H
#define GU7000_SERIAL_ASYNC_H

#include <Noritake_VFD_GU7000.h>
#include <avr/pgmspace.h>

//
// Lookup table
//
typedef struct _DELAY_TABLE
{
  long baud;
  unsigned short rx_delay_centering;
  unsigned short rx_delay_intrabit;
  unsigned short rx_delay_stopbit;
  unsigned short tx_delay;
} DELAY_TABLE;

//
// F_CPU ==  16000000
//
static const DELAY_TABLE PROGMEM table[] = 
{
  //  baud    rxcenter   rxintra    rxstop    tx
  { 115200,   1,         17,        17,       14,    },
  { 38400,    25,        57,        57,       55,    },	
  { 19200,    54,        117,       117,      115,   }, 
  { 9600,     114,       236,       236,      233,   },
};

const int XMIT_START_ADJUSTMENT = 5;

//template <unsigned NORITAKE_VFD_BAUD>
class GU7000_Serial_Async : public GU7000_Interface {

private:
	Noritake_VFD_GU7000 GU7000;
	uint16_t _rx_delay_centering;
	uint16_t _rx_delay_intrabit;
	uint16_t _rx_delay_stopbit;
	uint16_t _tx_delay;
	// private static method for timing
	static inline void tunedDelay(uint16_t delay){ 
	  uint8_t tmp=0;

	  asm volatile("sbiw    %0, 0x01 \n\t"
		"ldi %1, 0xFF \n\t"
		"cpi %A0, 0xFF \n\t"
		"cpc %B0, %1 \n\t"
		"brne .-10 \n\t"
		: "+r" (delay), "+a" (tmp)
		: "0" (delay)
		);
	}
	uint16_t _inverse_logic: 1;
	volatile uint8_t *_receivePortRegister;
	uint8_t _transmitBitMask;
	volatile uint8_t *_transmitPortRegister;
protected:
    unsigned OUT_PIN:4;
    unsigned BUSY_PIN:4;
    unsigned RESET_PIN:4;

	unsigned DEBUG_PIN:8;
    
public:
	
    GU7000_Serial_Async(long baudRate, uint8_t out, uint8_t busy, uint8_t reset):
        OUT_PIN(out), BUSY_PIN(busy), RESET_PIN(reset),
		_rx_delay_centering(0),  _rx_delay_intrabit(0),  _rx_delay_stopbit(0),  _tx_delay(0)
    {
		_rx_delay_centering = _rx_delay_intrabit = _rx_delay_stopbit = _tx_delay = 0;
		
		//Set TX
		_transmitBitMask = digitalPinToBitMask(out);
		uint8_t port = digitalPinToPort(out);
		_transmitPortRegister = portOutputRegister(port);
		
		//Set delay for TX
		for (unsigned i=0; i<sizeof(table)/sizeof(table[0]); ++i)
		{
			long baud = pgm_read_dword(&table[i].baud);
			if (baud == baudRate)
			{
			  _rx_delay_centering = pgm_read_word(&table[i].rx_delay_centering);
			  _rx_delay_intrabit = pgm_read_word(&table[i].rx_delay_intrabit);
			  _rx_delay_stopbit = pgm_read_word(&table[i].rx_delay_stopbit);
			  _tx_delay = pgm_read_word(&table[i].tx_delay);
			  break;
			}
		}
	}
	
    void init() {
        RAISE(RESET);
		DIRECTION(OUT,1);
    	DIRECTION(RESET,1);
    	DIRECTION(BUSY,0);
		//set model class		
		if(getModelClass==7003 || getModelClass==7903) {
			_inverse_logic = 0;
			RAISE(OUT);
		}else{
			_inverse_logic = 1;
			LOWER(OUT);
		}

				
    }
    void tx_pin_write(uint8_t pin_state){
		if (pin_state == LOW)
			*_transmitPortRegister &= ~_transmitBitMask;
		else
			*_transmitPortRegister |= _transmitBitMask;
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
    	LOWER(RESET);
    	_delay_ms(1);
    	RAISE(RESET);
    	_delay_ms(100);
		
		if(getModelClass==7003 || getModelClass==7903)
    		while (CHECK(BUSY));
		else
			while (!CHECK(BUSY));
		
	}
};

#endif
