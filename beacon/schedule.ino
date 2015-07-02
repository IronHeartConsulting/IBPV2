void handle_tick() {
  int schedule_ticks = wall_ticks - station.start_time;

  // TX starts happen inside interrupt.
  // IDs, TX stops, power changes, and band changes happen here

  FPPRINTRC(1,5,"   ");
  FPPRINTRC(1,5,wall_ticks);
  debug_print(F("Schedule: wall_ticks=")); debug_print_dec(wall_ticks);
  debug_print(F(" schedule_ticks=")); debug_print_dec(schedule_ticks);
  debug_print(F(" station.start_time=")); debug_print_dec(station.start_time);
  debug_print(F(" next_tx_click=")); debug_println_dec(next_tx_click);

  switch(schedule_ticks) {
  case -20:
    // not beaconing, so do GPS clock discipline.
    gps_discipline_clock(32768);
    break;

  case -1:
    next_tx_click = 5;
    FPPRINTRC(1,8,"        ");
    setband(20);
    setpower(50);
    break;

  case 0: case 10: case 20: case 30: case 40:
    // start sending ASAP.
    // send_id will not return until finished sending ID, somewhere between tick 2 and tick 4.
    // next tick we'll definitely see is tick 5.
    send_id(station.call);
    break;

  case 1: case 2: case 3: case 4:
    // Won't get here unless the ID is short, but if we do, make sure we don't start some other activity in our default case
    // by preventing it here.
    break;

  case 5: case 15: case 25: case 35: case 45:
    // definitely will be done with ID by now.
    // First DAH TX 100W was started by tick; keep it on for 750ms and prepare for next DAH and its power level.
    delay(750);
    txoff();
    next_tx_click++;
    setpower(40);
    break;

  case 6: case 16: case 26: case 36: case 46:
    // Second DAH TX 10W is already ongoing; keep it on for 750ms and prepare for next DAH and its power level.
    delay(750);
    txoff();
    next_tx_click++;
    setpower(30);
    break;

  case 7: case 17: case 27: case 37: case 47:
    // Third DAH TX 1W is already ongoing; keep it on for 750ms and prepare for next DAH and its power level.
    delay(750);
    txoff();
    next_tx_click++;
    setpower(20);
    break;

  case 8: case 18: case 28: case 38: case 48:
    // Fourth DAH TX 0.11W is already ongoing; keep it on for 750ms and prepare for band change.
    delay(750);
    txoff();
    next_tx_click = 255;
    break;

  case 9:
    // Fourth DAH is done.  9th second is silent.  ID on 10th second and start transmitting DAHs on 15th.
    next_tx_click = 15;
    setband(17);
    setpower(50);
    break;

  case 19:
    // Fourth DAH is done.  19th second is silent.  ID on 20th second and start transmitting DAHs on 25th.
    next_tx_click = 25;
    setband(15);
    setpower(50);
    break;

  case 29:
    // Fourth DAH is done.  29th second is silent.  ID on 30th second and start transmitting DAHs on 35th.
    next_tx_click = 35;
    setband(12);
    setpower(50);
    break;

  case 39:
    // Fourth DAH is done.  39th second is silent.  ID on 40th second and start transmitting DAHs on 45th.
    next_tx_click = 45;
    setband(10);
    setpower(50);
    break;

  case 60:
	setband(20);   // so we can listen to the start of the next cycle
    // not beaconing, so do GPS clock discipline.
    gps_discipline_clock(32768);
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
  
  Serial.print(F("new:next_tx_click:"));
  Serial.println(next_tx_click,DEC);
}
