#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

#define HMC5883_ADDRESS_MAG            (0x3C >> 1)

const int DATA_LENGTH = sizeof("DSAAAA.AAAAA,SOOOOO.OOOOO");

String gsGPSBuffer = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int giOffsets[12];
int giCheckSumPos;

String sCommand = "$GPRMC";
bool gbNewGPSData = false;
bool gbNewCompassData = false;

////////Control constants
const float NORTHEST = 33;
const float SOUTHEST = 39;
const char  LATITUDE_CHAR = 'S';
const float WESTEST = 64;
const float EASTEST = 54;
const char  LONGITUDE_CHAR = 'W';

const int SLAVE_ADDRESS = 8;

int gi = 0;
int giPos = 1;

String gsBlock = "";
String gsHeading = "";

/***************PROTOCOL*******************
GPS $GPRMC

ignore anything until you receive a valid "$GPRMC"
then start reading data devided by ','

Block n°  Example    Description
      0   $GPRMC     Always starts like this...
      1   220516     Time Stamp
      2   A          validity - A-ok, V-invalid
      3   5133.82    current Latitude
      4   N          North/South
      5   00042.24   current Longitude
      6   W          East/West
      7   173.8      Speed in knots
      8   231.8      True course
      9   130694     Date Stamp
      10  004.2      Variation
      11  W          East/West
      12  *70        checksum incuding this checksum

full block Example:
"$GPRMC,220516,A,5133.82,N,00042.24,W,173.8,231.8,130694,004.2,W*70"
********************************************/
