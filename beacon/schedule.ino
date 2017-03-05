void handle_tick() {
//   advancement of schedule ticks is done in the interrupt handler, so while the
//     radio is hogging the CPU, we still keep time
//***	schedule_ticks = (schedule_ticks + 1 ) % (3*60);

//   This approach issn't work, as it went negative when start_time > 100, 
//   and walls clock went 0.
//***  int schedule_ticks = wall_ticks - stations[slotindex].start_time;

  // TX starts happen inside interrupt.
  // IDs, TX stops, power changes, and band changes happen here
	if (!TXEnabled) {
		FPPRINTRC(1,5,"   ");
		FPPRINTRC(1,5,wall_ticks);
	}
	debug_print(F("Schedule: wall_ticks=")); debug_print_dec(wall_ticks);
	debug_print(F(" schedule_ticks=")); debug_println_dec(schedule_ticks);
	if (slotNotFound) {
		if ((wall_ticks - stations[slotindex].start_time) == -2) {
			slotNotFound = 0;
			schedule_ticks = 178;
		}
		else // still looking for wall_ticks == 0
			return;
	}

  switch(schedule_ticks) {
  case 160:
    // not beaconing, so do GPS clock discipline.
    gps_discipline_clock(32768);
    break;

  case 179:
	// check if our time in the penalty box is up
	if( skipEnabled) {
		if( (--remainingSkipCount) > 0) {
			FPPRINTRC(1,9,F("    "));
			FPPRINTRC(1,9,remainingSkipCount);
		}
		else { // penalty time is up, out of the box
			FPPRINTRC(1,0,"OPER");
			FPPRINTRC(1,9,"    ");
			skipEnabled = 0;
		}
	}
	else {  // not skipping
		TXEnabled = 1;
		FPPRINTRC(1,0,"TX           ");
    	setRadioMode(beaconMode);
    	setband(20);
		setpower(50);
	}
    break;

  case 0:
// 20m sequence
	runBand(20);
    break;

  case 10:
// 17 sequence
	runBand(17);
    break;

  case 20:
// 15 sequence
	runBand(15);
    break;

  case 30:
// 12 sequence
	runBand(12);
    break;

  case 40:
// 10 sequence
	runBand(10);
	if (!skipEnabled) 
		FPPRINTRC(1,0,"OPER");
	TXEnabled = 0;
    break;

  case 60:
	if (!skipEnabled)
		setband(20);   // so we can listen to the start of the next cycle
	// QSY to 14.070 USB so we can monitor PSK31
	setRadioMode(PSK31Mode);
    // not beaconing, so do GPS clock discipline.
	FPPRINTRC(1,7,F("         "));
    gps_discipline_clock(32768);
    break;

  case 140:
    // not beaconing, so do millis clock discipline for 10s
    gps_begin_milliclock_discipline();
    break;
  case 150:
    // not beaconing, finish 10s of millis clock discipline.
    gps_end_milliclock_discipline();
    break;

  default:
    break;
  }
  
  debug_println(F("sked exit"));
}

void runBand(byte band) {

	if (skipEnabled) 
		return;

	setband(band);
 	setpower(50);
 // This delay is tuned to match the V1 controllers, as meausred by FAROS
 // Don't change without re-calibrating.
 //    It's from the start of the epoch.  Changing the GPS discipline routine will affect this value
 //*** delay(352);
//
//   New value hand tuned at N6XG's place.  Probably due to adding code prior to the start of the loop
//   2/4 - N6XG suggests add 2ms more delay
//***	delay(136); 
//***	delay(248); 
//   delay now read from EEPROM
	delay(startDelay); 
	FPBLRED
    send_id();
	delay(218);
	// 1st long dash 100 watts
	KEYDOWN 
	delay(995);
	KEYUP
	// 2nd long dash 10 watts
	setpower(40);
	KEYDOWN 
	delay(995);
	KEYUP
	// 3rd long dash 1 watt
	setpower(30);
	KEYDOWN 
	delay(995);
	KEYUP
	// 4th long dash 100 milliwatts
	setpower(20);
	KEYDOWN 
	delay(995);
	KEYUP
	txoff();
	setpower(50);
	FPPRINTRC(1,7,F("         "));

	return;
}
