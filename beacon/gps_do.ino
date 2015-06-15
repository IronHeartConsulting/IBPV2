
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
  // Spend some time trying to get a GPS time.  If we fail, wall_ticks will be messed up.
  // If we succeed, wall_ticks will be right.
  while (--tries > 0) {
    // attend to GPS in event loop
    // not necessary all the time, but does need to happen to keep time sync
    if (gps_serial.available()) {
      char c = gps_serial.read();
      if (true) {
        if (true) {
          Serial.write(c);   Serial.flush();
        }

        if (gps.encode(c)) {
          Serial.println();
          // what about GPS.milliseconds?  Should we do this only if it is in a certain range?
          int year;
          byte month, day, hour, minute, second, hundredths;
          unsigned long fix_age;
          gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &fix_age);
          // about to write into wall_ticks but how do we know we're not about to interrupt?
          // maybe we should increment it by one and let interrupt write it itself.  that's assuming
          // the gps decode happens enough before the PPS.
          wall_ticks = ((minute * 60) + second) % (3*60);
          Serial.print(F("handle_gps_parsing says update wall_ticks to "));
          Serial.print(wall_ticks, DEC);
          Serial.print(F(" fix age "));
          Serial.print(fix_age);
          Serial.println(F("ms"));
          done = true;
          break;
        }
      }
    }
  }
  Serial.print(F("*** GPS Discipline clock done: "));
  Serial.println(done);
  Serial.flush();
  return done;
}

