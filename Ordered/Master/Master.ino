#include "Master.h"

void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  gsBuffer.reserve( DATA_LENGTH );
}

void loop()
{
  WAYPOINT destination;
  WAYPOINT myPosition;
  destination.Lat = -34.520333;
  destination.Lon = -58.502257;
  gsBuffer = "";

  // request data from slaves (GPS and Compass)
  bool bValid = RequestPosition( myPosition );
  Serial.print("Latitude: ");
  Serial.println(myPosition.Lat);
  Serial.print("Longitude: ");
  Serial.println(myPosition.Lon);
  
  float myHeading = -1;
  bValid = GetCompassData( myHeading );
  
  Serial.print("Heading: ");
  Serial.println(myHeading);
  
  //float distance = getDistance( myPosition, destination );
  //float destinationHeading = getDestHeading( myPosition, destination );

  delay(1000);
  //TODO: compare headings and decide where to turn, then move to position
}

float getDestHeading(const WAYPOINT pos, const WAYPOINT dest)
{
	float heading = 0.0;

	float pLon = radians(pos.Lon);
 	float dLon = radians(dest.Lon);
 	float dLat = radians(pos.Lat);
 	float pLat = radians(dest.Lat);

 	heading = atan2(sin(dLon-pLon)*cos(dLat),cos(pLat)*sin(dLat)-sin(pLat)*cos(dLat)*cos(dLon-pLon))*2;
 	// we need it in degrees so we son't multiply by π to then divide it by it
 	heading *= 180;// convert from radians to degrees

 	if( (int)heading < 0 )
 	{
 		heading += 360;//if the heading is negative then add 360 to make it positive
 	}

	return heading;
}

float GetCompassData( float &o_fHeading )
{
  bool bValid = true;
  
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

  if(Wire.available())
  {
    while(Wire.available()<6);//wait until we've got all data needed
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
  
    // store radians' heading
    o_fHeading = heading;
  }
  else
  {
    // no data received, ignore readings
    bValid = false;
  }

  return bValid;
}

bool RequestPosition( WAYPOINT &o_pos )
{
  bool bNewData = false;
	Wire.requestFrom(SLAVE_ADDRESS, DATA_LENGTH);

  delay(200);
  
	if (Wire.available())
	{
		bNewData = (bool)Wire.read();
	}

	if (bNewData)
	{
		gsBuffer = '1';//first newData flag
		while (Wire.available())
		{
			gsBuffer += (char)Wire.read();//read rest of the data
		}
		
		Serial.println(gsBuffer);
		//000000000011111111112222222222
		//012345678901234567890123456789
		//DSAAAA.AAAAA,SOOOOO.OOOOODC.CC
		int   iLatDeg = gsBuffer.substring(2, 4).toInt();
		float fLatMin = gsBuffer.substring(4, 12).toFloat();
		int   iLonDeg = gsBuffer.substring(14, 17).toInt();
		float fLonMin = gsBuffer.substring(17, 24).toFloat();
		o_pos.Lat = (float)iLatDeg + fLatMin / 60.0;
		o_pos.Lon = (float)iLonDeg + fLonMin / 60.0;

		if ('S' == gsBuffer.charAt(1))
		{
			o_pos.Lat *= -1;
		}

		if ('W' == gsBuffer.charAt(13))
		{
			o_pos.Lon *= -1;
		}
	}

	return bNewData;
}

float getDistance(const WAYPOINT pos, const WAYPOINT dest)
{
	float dlat = radians(dest.Lat - pos.Lat);
	float dlon = radians(dest.Lon - pos.Lon);
	float mylat = radians(pos.Lat);
	float destlat = radians(dest.Lat);

	float dist = sin(dlat / 2.0) * sin(dlat / 2.0) +
		sin(dlon / 2.0) * sin(dlon / 2.0) *
		cos(mylat) * cos(destlat);
	dist = 2 * atan2(sqrt(dist), sqrt(1.0 - dist));
	dist *= 6371000.0; //Converting to meters
	return dist;
}
