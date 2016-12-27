#include "Wire.h"

#define HMC5883_ADDRESS_MAG            (0x3C >> 1) 

void setup() {
    Wire.begin();
    Serial.begin(9600);
    pinMode(13, OUTPUT);
}

void loop() {  
  Wire.beginTransmission((byte)HMC5883_ADDRESS_MAG);
  Wire.write((uint8_t)0x02);
  Wire.write((uint8_t)0x00);
  Wire.endTransmission();
  Wire.beginTransmission((byte)HMC5883_ADDRESS_MAG);
  Wire.write((uint8_t)0x01);
  Wire.write((uint8_t)0x20);
  Wire.endTransmission();
  Wire.beginTransmission((byte)HMC5883_ADDRESS_MAG);
  Wire.write((uint8_t)0x03);
  Wire.endTransmission();
  
  Wire.requestFrom((byte)HMC5883_ADDRESS_MAG, (byte)6);
  while(Wire.available()<6);
  uint8_t xhi = Wire.read();
  uint8_t xlo = Wire.read();
  uint8_t zhi = Wire.read();
  uint8_t zlo = Wire.read();
  uint8_t yhi = Wire.read();
  uint8_t ylo = Wire.read();
  float x = (int16_t)(xlo | ((int16_t)xhi << 8));
  float y = (int16_t)(ylo | ((int16_t)yhi << 8));
  float z = (int16_t)(zlo | ((int16_t)zhi << 8));

  x = x / 1100 * 100;
  y = y / 1100 * 100;
  Serial.print("x: ");
  Serial.println( x );
  Serial.print("y: ");
  Serial.println( y );
  Serial.print("z: ");
  Serial.println( z );
  
  float heading = atan2(y, x);
  // Buenos aires declination is 8°47' (http://www.magnetic-declination.com/)
  // We need that in radians
  float declinationAngle = radians(8 + 47 / 60);
  heading += declinationAngle;
  
  // Correct for when signs are reversed.
  if(heading < 0)
  {
    heading += 2*PI;
  }
    
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
  {
    heading -= 2*PI;
  }

  Serial.print("Float Heading: ");
  Serial.println( heading * 100.0 );
  String gsHeading = String(heading, 2);

  Serial.print("Heading: ");
  Serial.println( gsHeading );
  
  delay(1000);
}
