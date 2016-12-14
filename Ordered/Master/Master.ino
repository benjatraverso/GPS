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
  
  bool bValid = RequestPosition( myPosition );
  
  float distance = getDistance( myPosition, destination );
  float destinationHeading = getDestHeading( myPosition, destination );
  float myHeading;
  bValid = getMyHeading( myHeading );
  if(bValid)
  {
  Serial.print("Heading: ");
  Serial.println(myHeading, 2);
  }
  delay(1000);
  //TODO: compare headings and decide where to turn and move to position
}

bool getMyHeading( float &headingCompass )
{
  //000000000011111111112222222222333
  //012345678901234567890123456789012
  //DSAAAA.AAAAA,SOOOOO.OOOOO;DCCC.CC
  bool bNewData = (bool)gsBuffer.charAt(26);
  Serial.println(gsBuffer);
  if( bNewData )
  {
    headingCompass = gsBuffer.substring(27, 32).toFloat();
  }
  return bNewData;
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

bool RequestPosition( WAYPOINT &o_pos )
{
	bool bNewData = false;
	Wire.requestFrom(SLAVE_ADDRESS, DATA_LENGTH);

	if (Wire.available())
	{
		bNewData = (bool)Wire.read();
	}

	if (bNewData)
	{
		gsBuffer = "";
		while (Wire.available())
		{
			gsBuffer += (char)Wire.read();//read rest of the data
		}

   
  Serial.println(gsBuffer);
		//000000000011111111112222222222333
		//012345678901234567890123456789012
		//DSAAAA.AAAAA,SOOOOO.OOOOO;DCCC.CC
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
