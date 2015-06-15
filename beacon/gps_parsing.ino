boolean handle_gps_parsing() {


  // if a sentence is received, we can check the checksum, parse it...

  if (GPS.newNMEAreceived()) {
    Serial.println(F("new NMEA sentence"));
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences! 
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false

    if (!GPS.parse(GPS.lastNMEA())) {  // this also sets the newNMEAreceived() flag to false
      return false;  // we can fail to parse a sentence in which case we should just wait for another
    }
  }

  // this is probably the place to reset the tick timer.
  // It's about 0.5 seconds before the PPS will happen, so set it to ticks-1 and let the interrupt increment it.

  if (GPS.fix) {
    //debug_print_gps();
    return true;
  } else {
    // Serial.println(F("*** NO GPS FIX ***"));
    return false;
  }
}
