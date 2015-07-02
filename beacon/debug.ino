#include "debug.h"

#if DEBUG

void setup_debug_print() {
  Serial.begin(115200);

  // Leonardo requires this.
  while (!Serial) {
  }
}

#endif
