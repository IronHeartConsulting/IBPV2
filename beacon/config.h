#ifndef CONFIG_H
#define CONFIG_H

//  compile time to inlude debugging versions of printf's and actually use them.
#define DEBUG 0
#define DEBUG_RADIO 0

// now read thru EEPROM value
//   value are index into stations array - 0 ->17
//   255 means unknown, don't TX
#define SLOTINDEX 14

#endif

