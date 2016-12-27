#include<Wire.h>

#define HMC5883_ADDRESS_MAG            (0x3C >> 1)

const int SLAVE_ADDRESS = 8;
const int DATA_LENGTH = sizeof("DSAAAA.AAAAA,SOOOOO.OOOOODC.CC");

bool gbNewData = false;
float gfLat;
float gfLon;

String gsBuffer = "";

struct WAYPOINT
{
	float Lat;
	float Lon;
};

struct ROUTE
{
	WAYPOINT value;
	ROUTE* next;
};

bool RequestPosition( WAYPOINT &o_myPosition );
bool GetCompassData( float & o_fHeading );
