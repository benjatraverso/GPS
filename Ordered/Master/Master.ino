#include "Master.h"

void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  gfBuffer.reserve( DATA_LENGTH );
}

void loop()
{
  bool bNewData = false;
  Wire.requestFrom(SLAVE_ADDRESS, DATA_LENGTH);

  if( Wire.available() )
  {
    bNewData = (bool)Wire.read();
  }

  if( bNewData )
  {
    gsBuffer = "";
    while( Wire.available() )
    {
      gsBuffer += (char)Wire.read(); // receive a byte as character
    }
  }

  if( bNewData )
  {
    //00000000001111111111
    //01234567890123456789
    //DSAAAA.AA,SOOOOO.OO*
    int   iLatDeg = gsBuffer.substring( 2, 4 ).toInt();
    float fLatMin = gsBuffer.substring( 4, 9 ).toFloat();
    int   iLongDeg = gsBuffer.substring( 11, 14 ).toInt();
    float fLongMin = gsBuffer.substring( 14, 19 ).toFloat();
    gfLat = ( float )iLatDeg + fLatMin / 60.0;
    gfLon = ( float )iLongDeg + fLatgMin / 60.0;
    
    if( 'S' == gsBuffer.charAt(0) )
    {
      gfLat *= -1;
    }

    if( 'W' == gsBuffer.charAt(10) )
    {
      gfLon *= -1;
    }
    
    Serial.print( "Data: " );
    Serial.print( "Latitude " );
    Serial.print( gfLat, 4 );
    Serial.print( "Longitude " );
    Serial.println( gfLon, 4 );
    gbNewData = false;
  }
  else
  {
    Serial.println( "Empty new package" );
  }
  delay( 5000 );
}