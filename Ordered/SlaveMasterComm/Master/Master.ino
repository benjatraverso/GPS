#include "Master.h"

void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
}

void loop()
{
  Wire.requestFrom(SLAVE_ADDRESS, DATA_LENGTH);    // request 6 bytes from slave device #8

  if( Wire.available() )
  {
    gbNewData = (bool)Wire.read();
  }
  if( gbNewData )
  {/*
    gfLat = Wire.parseFloat();
    Wire.read();
    gfLon = Wire.parseFloat();
    */
    //*
    while( Wire.available() )
    {
      Serial.println((float)Wire.read()); // receive a byte as character
    }
    //*/
  }

  if(gbNewData)
  {
    Serial.print("Data: ");
    Serial.print("Latitude ");
    Serial.print(gfLat);
    Serial.print("Longitude ");
    Serial.println(gfLon);
    gbNewData = false;
  }
  else
  {
    Serial.println("Empty new package");
  }
  delay(5000);
}
