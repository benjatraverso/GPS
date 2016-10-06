#include "GoToWayointStopOnObject.h"

void setup()
{
  pinMode( FWD, OUTPUT );
  pinMode( BWD, OUTPUT );
  pinMode( EN,  OUTPUT );
  
  Serial.begin( 9600 );  // Initialize serial: 
  myservo.attach( SRV );
  state = eWaitingForWaypoint;
}

void loop()
{
  //
  DoStep();
  GetNextStep();
}

void GetNextStep( void )
{
  switch(state)
  {
    /*
    case StepName:
    {
      state = eNextStateName;
      break;
    }
    */
    case eWaitingForWaypoint:
    {
      if( gValidWaypoint )
      {
        state = eCalculateRoute;
      }
      else
      {
        state = eWaitingForWaypoint;
      }
      break;
    }
  }
}


void DoStep( void )
{
  switch(state)
  {
    /*
    case StepName:
    {
      Step_StepName();
      break;
    }
    */
    case eWaitingForWaypoint:
    {
      Step_WaitingForWaypoint();
      break;
    }
    case eCalculateRoute:
    {
      Step_CalculateRoute();
      break;
    }
  }
}
  /*OLD CODE
  if( Serial.available() > 0 )
  {
    char cReading = Serial.read();
    passedTime = millis();
    switch( cReading )
    {
      case 'A':
      {
        int iAngle = Serial.parseInt();
        // servo can go from 0 to 180, but this car's stearing finds a mechanical limitation further than 32-160.
        iAngle = constrain( iAngle, 32, 160 );
        myservo.write( iAngle );
        break;
      }
      case 'G':
      {
        unsigned int iGear = Serial.parseInt();
        if( iGear )
        {
          digitalWrite( BWD, LOW );
          digitalWrite( FWD, HIGH );
        }
        else
        {
          digitalWrite( FWD, LOW );
          digitalWrite( BWD, HIGH );
        }

        break;
      }
      case 'T':
      {
        int iThrottle = Serial.parseInt();
        unsigned int uiThrottle = constrain( iThrottle, 0, 255 );
        analogWrite( EN, uiThrottle );
        break;
      }
      default:
      {
        // Not a valid command. Ignore.
        break;
      }
    }
  }
  else if(2000 < ( millis() - passedTime ) )
  {
    //if no data available for longer than 2 seconds kill everything
    digitalWrite( FWD, LOW );
    digitalWrite( BWD, LOW );
    analogWrite( EN, LOW );
    myservo.write( 90 );
  }
}
*/