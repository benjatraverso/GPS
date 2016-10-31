/*
 * dummy master acts as both master and gps to test the slave sends data correctly as received from GPS
 * a valid data from gps and an invalid one to see if it reaches the master back.
 * $GPRMC,220516,A,3633.82,S,06042.24,W,173.8,231.8,130694,004.2,W*66
 * $GPRMC,220516,A,3633.82,N,06042.24,W,173.8,231.8,130694,004.2,W*66 //invalid north coordinate
 */

#include "DummyMaster.h"

void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(4800);  // start serial for output
  gsBuffer.reserve(50);
  pinMode(13, OUTPUT);
}

void loop()
{
  if(toogleValidInvalid())
  {
    Serial.println("$GPRMC,220516,A,3633.82,S,06042.24,W,173.8,231.8,130694,004.2,W*66");
  }
  else
  {
    Serial.println("$GPRMC,220516,A,3633.82,N,06042.24,W,173.8,231.8,130694,004.2,W*66");
  }
  delay(1000);
  
  Wire.requestFrom(SLAVE_ADDRESS, DATA_LENGTH);    // request 6 bytes from slave device #8

  if( Wire.available() )
  {
    gbNewData = (bool)Wire.read();
  }

  if( gbNewData )
  {
    gsBuffer = "";
    while( Wire.available() )
    {
      gsBuffer =+ Wire.read(); // receive a byte as character
    }
    if(DATA_LENGTH == sizeof(gsBuffer))
    {
      digitalWrite(13, HIGH);
    }
  }

  delay(1000);
  digitalWrite(13, LOW);
}

bool toogleValidInvalid(void)
{
  gbToogle = !gbToogle;
  return gbToogle;
}

