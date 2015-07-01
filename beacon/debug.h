#ifndef DEBUG_H
#define DEBUG_H

#if DEBUG
#define debug_println(x) Serial.println(x)
#define debug_print(x) Serial.print(x)
#define debug_newline(x) Serial.println()
#else
#define debug_println(x)
#define debug_print(x)
#define debug_newline(x) Serial.println()
#endif

#endif
