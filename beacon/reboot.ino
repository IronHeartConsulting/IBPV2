#include <avr/wdt.h>
#include "beacon.h"

void reboot() {

	do                          
	{                           
		wdt_enable(WDTO_15MS);  
	    for(;;)                 
	    {                       
	    }                       
	} while(0);

}

void wdt_init(void) __attribute__((naked)) __attribute__((section(".init3")));
// Function Implementation
void wdt_init(void)
{
    MCUSR = 0;
    wdt_disable();

    return;
}
