#include <Servo.h>

const unsigned int SRV = 9;

const unsigned int FWD = 10;
const unsigned int BWD = 4;
const unsigned int EN =  3;

const unsigned int ECHO = 11;
const unsigned int TRIG = 12;

Servo myservo;
unsigned int passedTime = 0;

enum states
{
	eIdle = 0;
	ewa
}