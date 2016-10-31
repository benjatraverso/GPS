#include "Slave.h"

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
    Serial.println(gsGPSBuffer);
    bool bValid = true;
    bValid = ValidateSentence();
    bValid = bValid && ValidateBlock();
    bValid = bValid && StoreData();
    
    gsGPSBuffer = "";
    memset(giOffsets, 0, 12);//clear the offset's array
    gi = 0;
    giPos = 1;
    stringComplete = false;
  }
}

bool StoreData( void )
{
  //      0         1 2       3 4        5 6   7   8      9   1011
  //$GPRMC,hhmmss.ss,A,llll.ll,a,yyyyy.yy,a,x.x,x.x,ddmmyy,x.x,a*hh
  char cLat = gsGPSBuffer.charAt(giOffsets[3]);
  int iLatDeg = gsGPSBuffer.substring(giOffsets[2], giOffsets[2]+2).toInt();//first two numbers are the degrees
  int iLatMin = gsGPSBuffer.substring(giOffsets[2]+2, giOffsets[2]+2+2).toInt();
  int iLatSeg = gsGPSBuffer.substring(giOffsets[2]+2+3, giOffsets[3]-1).toInt();//rest is minutes with decimals

  char cLong = gsGPSBuffer.charAt(giOffsets[5]);
  int iLongDeg = gsGPSBuffer.substring(giOffsets[4], giOffsets[4]+3).toInt();
  int iLongMin = gsGPSBuffer.substring(giOffsets[4]+3, giOffsets[4]+3+2).toInt();
  int iLongSeg = gsGPSBuffer.substring(giOffsets[4]+3+2+1, giOffsets[5]-1).toInt();

  bool bValid = true;

  Serial.println(iLatDeg);
  Serial.println(iLongDeg);
  bValid = iLatDeg > NORTHEST && iLatDeg < SOUTHEST;
  bValid = bValid && cLat == LATITUDE_CHAR;
  bValid = bValid && (iLongDeg < WESTEST && iLongDeg > EASTEST);
  bValid = bValid && cLong == LONGITUDE_CHAR;

  if(bValid)
  {
    //SXXXX.XX,SYYYYY.YY
    gbNewData = true;
    gcLat = cLat;
    giLatDeg = iLatDeg;
    giLatMin = iLatMin;
    giLatSeg = iLatSeg;
  
    gcLong = cLong;
    giLongDeg = iLongDeg;
    giLongMin = iLongMin;
    giLongSeg = iLongSeg;
  }
  else
  {
    Serial.println("Data validation failed: ");
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

bool ValidateSentence( void )
{
  //ignore other sentence different from $gprmc
  return sCommand == gsGPSBuffer.substring(0, 6);
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

void requestEvent( void )
{
  Wire.write((char)gbNewData); //always indicate if we have new data
  if(gbNewData)
  {
    Wire.write((char)gbNewData);

    Wire.write(gcLat);
    Wire.write(giLatDeg);
    Wire.write(giLatMin);
    Wire.write(giLatSeg);

    Wire.write(gcLong);
    Wire.write(giLongDeg);
    Wire.write(giLongMin);
    Wire.write(giLongSeg);

    gbNewData = false;
  }
}
