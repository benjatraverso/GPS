#include "Master.h"

void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(4800);  // start serial for output
  gsBuffer.reserve(50);
}

void loop()
{
  Wire.requestFrom(SLAVE_ADDRESS, DATA_LENGTH);    // request 6 bytes from slave device #8

  if( Wire.available() )
  {
    gbNewData = (bool)Wire.read();
  }
  if( gbNewData )
  {
    while( Wire.available() )
    {
      Serial.print( Wire.read() ); // receive a byte as character
    }
  }
  Serial.println('.');
  delay(1000);
}

void printBuffer(void)
{
  Serial.println(gsBuffer);
  //0123456789 0123456789
  //SXXXX.XXXX,SXXXX.XXXX
  /*
  giLatDeg = gsBuffer.substring(1, 3).toInt;
  if('S' == gsBuffer.substring(0) )
  {
    giLatDeg =* (-1);
  }

  unsigned int uiLongIndex = gsBuffer.indexOf(',') + 1;
  giLongDeg = gsBuffer.substring(uiLongIndex, uiLongIndex + 2).toInt;
  if('S' == gsBuffer.substring(0) )
  {
    giLatDeg =* (-1);
  }
  */
  /*
  unsigned int uiLongIndex = gsBuffer.indexOf(',') + 1;

  int iLatDeg = gsBuffer.substring(1, 3).toInt;
  int iLatMinInt = gsBuffer.substring(3, 5).toInt;
  int iLatMinDec = gsBuffer.substring(6, uiLongIndex - 1 ).toInt;
  float fLatDec = ((float)iLatMinInt + (float)iLatMinDec/60.0) / 60.0;
  Serial.print("Latitude Decimal part: ");
  Serial.println(fLatDec);
  gfLat = (float)iLatDeg + fLatDec;
  if('S' == gsBuffer.substring(0) )
  {
    gfLat =* (-1.0);
  }

  int iLongtDeg = gsBuffer.substring(uiLongIndex, uiLongIndex + 3).toInt;
  int iLongMinInt = gsBuffer.substring(uiLongIndex + 3, uiLongIndex + 5).toInt;
  int iLongMinDec = gsBuffer.substring(uiLongIndex + 6 ).toInt;
  float fLongDec = ((float)iLongMinInt + (float)iLongMinDec/60.0) / 60.0;
  Serial.print("Longitude Decimal part: ");
  Serial.println(fLongDec);
  gfLong = (float)iLongtDeg + fLongDec;
  if('W' == gsBuffer.substring(uiLongIndex) )
  {
    gfLong =* (-1.0);
  }
  
  //estas son las variables finales de 
  Serial.print("Complete Lat: ");
  Serial.println(gfLat);
  Serial.print("Complete Long: ");
  Serial.println(gfLong);
  */
}
