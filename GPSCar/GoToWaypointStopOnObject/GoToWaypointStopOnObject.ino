#include "Main.h"

void setup()
{
  pinMode( FWD, OUTPUT );
  pinMode( BWD, OUTPUT );
  pinMode( EN,  OUTPUT );
  
  Serial.begin( 9600 );  // Initialize serial: 
  myservo.attach( SRV );
  gState = eReadingCommands;
}

void loop()
{
  DoStep();
  GetNextStep();
}

void GetNextStep( void )
{
  switch( gState )
  {
    /*for copy and pasting
    case StepName:
    {
      gState = eNextStateName;
      break;
    }
    */
    case eReadingCommands:
    {
      if( gbWaypointReceived )
      {
        gState = eValidateWaypoint;
      }
      break;
    }

    case eValidateWaypoint:
    {
      if( gbValidWaypoint )
      {
        gState = eMoveToWaypoint;
      }
      else
      {
        gState = eReadingCommands;
      }
      break;
    }

    default:
    {
      gState = eReadingCommands;
      break;
    }
  }
}


void DoStep( void )
{
  switch( gState )
  {
    /*
    case StepName:
    {
      Step_StepName();
      break;
    }
    */
    case eReadingCommands:
    {
      Step_ReadingCommands();
      break;
    }
    case eValidateWaypoint:
    {
      Step_ValidateWaypoint();
      break;
    }
  }
}
