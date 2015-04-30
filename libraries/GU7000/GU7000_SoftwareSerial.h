#ifndef GU7000_SOFTWARESERIAL_H
#define GU7000_SOFTWARESERIAL_H

#include <Noritake_VFD_GU7000.h>
#include <avr/types.h>
#include <SoftwareSerial.h>

class 
GU7000_SoftwareSerial 
    : public GU7000_Interface
{
    public:
        GU7000_SoftwareSerial(long baud, uint8_t out, uint8_t busy, uint8_t reset) :
                                port(out - 1, out), OUT_PIN(out), BUSY_PIN(busy), RESET_PIN(reset)
        {
            port.begin(baud);
        };

        void
        init()
        {
            RAISE(RESET);
            DIRECTION(OUT, 1);
            DIRECTION(RESET, 1);
            DIRECTION(BUSY, 0);
      		if(getModelClass==7003 || getModelClass==7903) {
    			_inverse_logic = 0;
		    }else{
			    _inverse_logic = 1;
		    }
        };

        void
        write(uint8_t data)
        {
            if(getModelClass==7003 || getModelClass==7903 || getModelClass == 7053)
                while (CHECK(BUSY));
            else
                while (!CHECK(BUSY));
            port.write(data);
        };

    void 
    hardReset() 
    {
        init();
    	LOWER(RESET);
    	_delay_ms(1);
    	RAISE(RESET);
    	_delay_ms(100);

		if(getModelClass==7003 || getModelClass==7903 || getModelClass == 7053)
    		while (CHECK(BUSY));
		else
			while (!CHECK(BUSY));
	}

    private:
        SoftwareSerial port;
        uint8_t OUT_PIN;
        uint8_t BUSY_PIN;
        uint8_t RESET_PIN;
        uint8_t _inverse_logic;
};

#endif
