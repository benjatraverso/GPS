#include <Servo.h>

const unsigned int SRV = 9;

const unsigned int FWD = 10;
const unsigned int BWD = 4;
const unsigned int EN =  3;

const unsigned int ECHO = 11;
const unsigned int TRIG = 12;

Servo myservo;
unsigned int passedTime = 0;

///////////////GLOBAL VARIABLES//////////////////
unsigned bool gValidWaypoint = false;
unsigned int gState = eIdle;

enum states
{
	eIdle = 0,
	eWaitingForWaypoint,
	eCalculateRoute
};

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
      12  *70        checksum

full block Example:
"$GPRMC,220516,A,5133.82,N,00042.24,W,173.8,231.8,130694,004.2,W*70"
********************************************/