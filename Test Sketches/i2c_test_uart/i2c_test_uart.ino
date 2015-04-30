#include <Wire.h>

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(1000);
  }
  Wire.begin();

}

void loop() {
  int bytesRead = 0, i2cResult;
  uint8_t data;
  // set to basic register set
  Wire.beginTransmission(0x48);
  Wire.write(0x04<<3);  // set MCR to zero
  Wire.write(0x00);
  i2cResult = Wire.endTransmission();
  Serial.print("clear MCR - I2C Result:");
  Serial.println(i2cResult);
  // write 0x55 to the scartch pad register
  Wire.beginTransmission(0x48);
  Wire.write(0x07<<3);
  Wire.write(0x55);
  i2cResult = Wire.endTransmission();
  Serial.print("I2C Result:");
  Serial.println(i2cResult);
  // read the scratch pad reigster
  Wire.beginTransmission(0x48);
  Wire.write(0x07<<3);
  i2cResult = Wire.endTransmission();
  Serial.print("I2C Result:");
  Serial.println(i2cResult);
  bytesRead = Wire.requestFrom(0x48,1,true);
  Serial.print("# bytes returned:");
  Serial.println(bytesRead);
  data = Wire.read();
  Serial.print("1st byte of data:");
  Serial.println(data);
  delay(5000);
}
