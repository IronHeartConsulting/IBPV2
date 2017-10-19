#ifndef DEBUG_H
#define DEBUG_H

#if DEBUG
#define debug_println(x) Serial.println(x)
#define debug_println_dec(x) Serial.println(x,DEC)
#define debug_print(x) Serial.print(x)
#define debug_print_dec(x) Serial.print(x,DEC)
#define debug_newline(x) Serial.println()
#else
#define debug_println(x)
#define debug_println_dec(x)
#define debug_print(x)
#define debug_print_dec(x)
#define debug_newline(x) 
#endif

#endif
