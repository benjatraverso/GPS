#include "Master.h"

void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  gsBuffer.reserve( DATA_LENGTH );
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
    //0000000000111111111122222
    //0123456789012345678901234
    //DSAAAA.AAAAA,SOOOOO.OOOOO*
    int   iLatDeg = gsBuffer.substring( 2, 4 ).toInt();
    float fLatMin = gsBuffer.substring( 4, 12 ).toFloat();
    int   iLonDeg = gsBuffer.substring( 14, 17 ).toInt();
    float fLonMin = gsBuffer.substring( 17, 24 ).toFloat();
    gfLat = ( float )iLatDeg + fLatMin / 60.0;
    gfLon = ( float )iLonDeg + fLonMin / 60.0;
    
    if( 'S' == gsBuffer.charAt(1) )
    {
      gfLat *= -1;
    }

    if( 'W' == gsBuffer.charAt(13) )
    {
      gfLon *= -1;
    }
    
    Serial.print( "Data: " );
    Serial.print( "Latitude " );
    Serial.print( gfLat, 6 );
    Serial.print( " - Longitude " );
    Serial.println( gfLon, 6 );
    gbNewData = false;
  }
  else
  {
    Serial.println( "Empty new package" );
  }
  delay( 5000 );
}
