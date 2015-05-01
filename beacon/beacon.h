
/*

	beacon.h - defines shared across all files

*/

#ifndef BEACON_H
#define BEACON_H


// bunch of "beacon" defines
#define GPSECHO false
#define CWLINE 5 		// pin connected ot CW keyer line - for morse code generation
#define PTTLINE 12		// go to TX
#define PTTON LOW		// logic value to enable TX
#define PTTOFF HIGH
#define DLEAYTIMER 60	// number of seconds to wait for USB serial monitor to connect
#define PPS 3			// pin connected to GPS PPS interrupt input
#define LED 13			// LED on PCA

#endif
