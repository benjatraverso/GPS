#include "Master.h"

void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  gsBuffer.reserve( DATA_LENGTH );
}

void loop()
{
	;
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
