#include <Wire.h>

const int SLAVE_ADDRESS = 8;
const int DATA_LENGTH = sizeof("DSAAAA.AAAAA,SOOOOO.OOOOO*");

bool gbNewData = false;
float gfLat;
float gfLon;

String gsBuffer = "";

