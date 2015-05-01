void debug_print_gps() {
  Serial.print(F("Date: "));
  Serial.print(F("20"));
  Serial.print(GPS.year, DEC);
  Serial.print('-');
  zeroprint(GPS.month);
  Serial.print('-');
  zeroprint(GPS.day); 
  Serial.print(F("T"));
  zeroprint(GPS.hour); 
  Serial.print(':');
  zeroprint(GPS.minute); 
  Serial.print(':');
  zeroprint(GPS.seconds); 
  Serial.print('.');
  Serial.println(GPS.milliseconds);
  Serial.print(F("Fix: ")); 
  Serial.print((int)GPS.fix);
  Serial.print(F(" quality: ")); 
  Serial.println((int)GPS.fixquality); 
  if (GPS.fix) {
    Serial.print(F("Location: "));
    Serial.print(GPS.latitude, 4); 
    Serial.print(GPS.lat);
    Serial.print(F(", ")); 
    Serial.print(GPS.longitude, 4); 
    Serial.println(GPS.lon);

    Serial.print(F("Speed (knots): ")); 
    Serial.println(GPS.speed);
    Serial.print(F("Angle: ")); 
    Serial.println(GPS.angle);
    Serial.print(F("Altitude: ")); 
    Serial.println(GPS.altitude);
    Serial.print(F("Satellites: ")); 
    Serial.println((int)GPS.satellites);
  }
}


void zeroprint(byte z) {
  if (z < 10) Serial.print('0');
  Serial.print(z, DEC);
}


