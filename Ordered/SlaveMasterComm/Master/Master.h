#include <Wire.h>

const int SLAVE_ADDRESS = 8;
const int DATA_LENGTH = sizeof("DSAA.AAAA,SOOO.OOO*");

bool gbNewData = false;
float gfLat;
float gfLon;
/*
int		giLatDeg;
float	gfLatMin;
int		giLongDeg;
float	gfLongMin;
bool	gbNewData;
*/
