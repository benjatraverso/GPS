#include<Wire.h>
#include<LiquidCrystal.h>

const int SLAVE_ADDRESS = 8;
const int DATA_LENGTH = sizeof("DSAAAA.AAAAA,SOOOOO.OOOOO*");

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
}