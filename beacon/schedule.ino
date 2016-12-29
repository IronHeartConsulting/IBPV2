//   schedule.ino
//
//     called by looo() on a clock tick
//     calls the correct radio fucntions at the right time
//
#include "beacon.h"
#include "softreset.h"
void handle_tick() {
  int schedule_ticks = wall_ticks - stations[slotindex].start_time;

  // IDs, TX stops, power changes, and band changes happen here
	if (slotindex == 255) {  // don't know our TX slot, just exit
  		FPPRINTRC(1,5,F("Time slot not set"));
		return;
	}
		
  FPPRINTRC(1,5,F("           "));
  FPPRINTRC(1,5,wall_ticks);
  debug_print(F("Schedule: wall_ticks=")); debug_print_dec(wall_ticks);
  debug_print(F(" schedule_ticks=")); debug_print_dec(schedule_ticks);
  // debug_print(F(" station.start_time=")); debug_print_dec(station.start_time);
  debug_print(F(" next_tx_click=")); debug_println_dec(next_tx_click);

//  allow menu to run
	runMenu();

  switch(schedule_ticks) {
  case -20:
    // not beaconing, so do GPS clock discipline.
    gps_discipline_clock(32768);
    break;

  case -1:
    next_tx_click = 5;
    FPPRINTRC(1,8,F("        "));
    setRadioMode(beaconMode);
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
    break;

  case 60:
	setband(20);   // so we can listen to the start of the next cycle
	// QSY to 14.070 USB so we can monitor PSK31
	setRadioMode(PSK31Mode);
    // not beaconing, so do GPS clock discipline.
    gps_discipline_clock(32768);
    break;

  case 70:
	uptime++;
	debug_print(F("uptime:"));  debug_print_dec(uptime);
	if (uptime > MAX_UPTIME) { //  just reboot
		soft_restart();
	}

	break;

  case 168:
    // not beaconing, so do millis clock discipline for 10s
    gps_begin_milliclock_discipline();
    break;
  case 170:
    // not beaconing, finish 10s of millis clock discipline.
    gps_end_milliclock_discipline();
    break;

  default:
    break;
  }
  
  debug_println(F("sked exit"));
}

void runBand(byte band) {

	FPPRINTRC(1,0,F("TX           "));
	setband(band);
	setpower(50);
	setALCPwr(LOW);  // make sure ALC ctl voltage is shutdown or off
//
//    delay from start of epoch 1PPS tick till we start sending CW.
//
// This delay is tuned to match the V1 controllers, as meausred by FAROS
// Don't change without re-calibrating.
//    It's from the start of the epoch.  Changing the GPS discipline routine will affect this value
//*** delay(352);
//
//   New value hand tuned at N6XG's place.  Probably due to adding code prior to the start of the loop
	delay(134); 

	// send the CW ID
    send_id(stations[slotindex].call);
	delay(250);
	// 1st long dash 100 watts
	KEYDOWN 
	delay(900);
	KEYUP
	// 2nd long dash 10 watts
	setpower(40);
	KEYDOWN 
	delay(900);
	KEYUP
	// 3rd long dash 1 watt
	setpower(30);
	KEYDOWN 
	delay(900);
	KEYUP
	// 4th long dash 100 milliwatts
	setpower(20);
	setALCPwr(HIGH);
	KEYDOWN 
	delay(900);
	KEYUP
	txoff();

	return;
}
