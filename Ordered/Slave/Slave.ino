#include "UARTtoI2C.h"

void setup()
{
  // initialize serial:
  Serial.begin(4800);
  // reserve 200 bytes for the inputString:
  gsGPSBuffer.reserve(200);
  Wire.begin(SLAVE_ADDRESS);
  Wire.onRequest(requestEvent);
}

void loop()
{
  // print the string when a newline arrives:
  if(stringComplete)
  {
    bool bValid = true;
    bValid = ValidateBlock();
    bValid = bValid && StoreData();
    
    gsGPSBuffer = "";
    memset(giOffsets, 0, 12);//clear the offset's array
    int gi = 0;
    int giPos = 1;
    stringComplete = false;
    
    Serial.print("Latitude: ");
    Serial.print(giLatDegrees);
    Serial.print("Deg");
    Serial.print(gfLatMin);
    Serial.print("Min");
    Serial.println(gcLat);
    Serial.print("Longitude: ");
    Serial.print(giLongDegrees);
    Serial.print("Deg");
    Serial.print(gfLongMin);
    Serial.print("Min");
    Serial.println(gcLong);
  }

}

bool StoreData( void )
{
  char cLat = gsGPSBuffer.charAt(giOffsets[3]);
  int iLatDegrees = gsGPSBuffer.substring(giOffsets[2], giOffsets[2]+2).toInt();//first two numbers are the degrees
  float fLatMin = gsGPSBuffer.substring(giOffsets[2]+2, giOffsets[3]-1).toFloat();//rest is minutes with decimals
  
  char cLong = gsGPSBuffer.charAt(giOffsets[5]);
  int iLongDegrees = gsGPSBuffer.substring(giOffsets[4], giOffsets[4]+2).toInt();
  float fLongMin = gsGPSBuffer.substring(giOffsets[4]+2, giOffsets[5]-1).toFloat();

  bool bValid = true;

  bValid = iLatDegrees > NORTHEST && iLatDegrees < SOUTHEST;
  bValid = bValid && cLat == LATITUDE_CHAR;
  bValid = bValid && (iLongDegrees < WESTEST && iLongDegrees > EASTEST);
  bValid = bValid && cLong == LONGITUDE_CHAR;

  if(bValid)
  {    
    gcLat = cLat;
    giLatDegrees = iLatDegrees;
    gfLatMin = fLatMin;
    
    gcLong = cLong;
    giLongDegrees = iLongDegrees;
    gfLongMin = fLongMin;
  }
  else
  {
    Serial.println("Data validation failed");
    //ignore all readings...
  }
  
  return bValid;
}

bool ValidateBlock( void )
{
  bool bValid = gsGPSBuffer.length() == gsGPSBuffer.substring(giCheckSumPos).toInt();
  if(!bValid)
  {
    Serial.println("Checksum validation failed");
  }
  return bValid;
}

void serialEvent()
{  
  while(Serial.available())
  {
    int iChar = Serial.read();
    if(iChar == ',')
    {
      giOffsets[gi++] = giPos;
    }
    else if( iChar == '*' )
    {
      giCheckSumPos = giPos;
    }

    if (iChar == '\n')
    {
      stringComplete = true;
      // we want the checksum to be the last thing in the buffer
      break;
    }
    else
    {
      gsGPSBuffer += (char)iChar;
    }
    giPos++;
  }
}

void requestEvent()
{
  Wire.write(giLatDegrees);
  Wire.write(gfLatMin);
  Wire.write(gcLat);
  Wire.write(giLongDegrees);
  Wire.write(gfLongMin);
  Wire.write(gcLong);
}