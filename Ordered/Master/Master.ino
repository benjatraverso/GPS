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

	While( !RequestPosition( &myPosition ) )
	{
		// if no position we cannot proceed
		delay(500);// wait a little and request again
	}

	float distance = getDistance( myPosition, destination );
	float destinationHeading = getDestHeading( myPosition, destination );
	while( !getMyHeading( &myHeading ) )
	{
		// try again, we need to know where are we heading to
		delay(500);
	}

	//TODO: compare headings and decide where to turn and move to position
}

bool getMyHeading( int &headingCompass )
{
	bool bValid = true;
	String headingData = "";

	Wire.beginTransmission(compassAddress);        //the wire stuff is for the compass module
	Wire.send("A");
	Wire.endTransmission();
	delay(10);
	Wire.requestFrom(compassAddress, 2);
	i = 0;

	while(Wire.available())
	{
		headingData += Wire.receive();
	}
	
	int heading = headingData.parseInt();

 	int pracheading = headingValue / 10;      // this is the heading of the compass
	if( pracheading > 0 )
	{
		headingCompass = pracheading;
	}
	else
	{
		bValid = false;
	}

	return bValid;
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

		//0000000000111111111122222
		//0123456789012345678901234
		//DSAAAA.AAAAA,SOOOOO.OOOOO*
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
