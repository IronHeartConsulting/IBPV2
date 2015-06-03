
uint16_t intrCount;
void setup() {
  // put your setup code here, to run once:
  attachInterrupt(digitalPinToInterrupt(7), ISR_D7, CHANGE);
  intrCount = 0;
}

void ISR_D7() {
  intrCount++;
  return;
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("interrupt count:");
  Serial.println(intrCount);
  delay(10000);

}
