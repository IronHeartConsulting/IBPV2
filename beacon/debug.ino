#include "debug.h"

#if DEBUG

void setup_debug_print() {
  Serial.begin(115200);

  // Leonardo requires this.
  while ((--tryCount) > 0) {
	if (Serial) {
		break;
	}
	delay(1000);  // wait 1 second between tests for serial port working
  }
}

#endif
