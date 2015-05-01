
void gps_begin_milliclock_discipline() {
  disciplining_milliclock = true;
}

void gps_end_milliclock_discipline() {
  disciplining_milliclock = false;
  if (millis_per_second != 0) {
    seven_fifty_millis = (millis_per_second * 3 / 4);
    Serial.print(F("Disciplined 750 millis to ")); Serial.println(seven_fifty_millis);
  }
}

boolean gps_discipline_clock(long tries) {
  boolean done = false;
  Serial.println(F("*** GPS Discipline clock start"));
  Serial.flush();
  // Spend some time trying to get a GPS time.  If we fail, clicks will be messed up.
  // If we succeed, clicks will be right.
  while (--tries > 0) {
    // attend to GPS in event loop
    // not necessary all the time, but does need to happen to keep time sync
    char c = GPS.read();
    if (true) {
      if (c) {
        Serial.write(c);   Serial.flush();
      }
    }

    if (handle_gps_parsing()) {
      // what about GPS.milliseconds?  Should we do this only if it is in a certain range?
      clicks = ((GPS.minute * 60) + GPS.seconds) % (3*60);
      Serial.print(F("handle_gps_parsing says update clicks to "));
      Serial.println(clicks, DEC);
      Serial.flush();
      done = true;
      break;
    }
  }
  Serial.print(F("*** GPS Discipline clock done: "));
  Serial.println(done);
  Serial.flush();
  return done;
}
