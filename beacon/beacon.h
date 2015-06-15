
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
#define PPS 7			// pin connected to GPS PPS interrupt input
#define LED 13			// LED on PCA
#define GPSRxD 11                // GPS software serial RxD pin
#define GPSTxD 8               // GPS software serial TxD pin - NOTE - non-standard usage, and incompat with Adafruit shield wiring

// radio (CI-V) defines
#define radio_baudrate 4800
#define CIV_CA 0xE0
#define CIV_RA 0x76

#endif
