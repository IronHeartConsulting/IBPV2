#include "beacon.h"
#include "config.h"
#include "BCNDebug.h"
#include <bounce2.h>


int runMenu() {

	if (menuBtn.read() == LOW ) {
		FPBLBLUE
	}
	else {
		FPBLGREEN
	}

	return(0);

}
