void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  while (!Serial) {
  }
  
  Serial.println(F("minute overflow test V1.0"));
  byte minute, wall_ticks;
  int i;
  for (i=0; i<70; i++) {
    minute = i;
    wall_ticks = ((minute*60)+5) % (3*60);
    Serial.print (F("minute:"));
    Serial.print (minute,DEC);
    Serial.print (F("  wall_ticks:"));
    Serial.println (wall_ticks,DEC);
  }
  Serial.println(F("done"));
  Serial.flush();

}

void loop() {
  // put your main code here, to run repeatedly:

}
