#include "Slave.h"

void setup()
{
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  gsGPSBuffer.reserve(200);
  gsBlock.reserve(2*23);
  Wire.begin(SLAVE_ADDRESS);
  Wire.onRequest(requestEvent);
}

void loop()
{
  // print the string when a newline arrives:
  if(stringComplete)
  {
    //Serial.println(gsGPSBuffer);
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

  if(gbNewData && bShow)
  {
    Serial.print("Block: ");
    //SXXXX.XX,SYYYYY.YY
    Serial.println(gsBlock);
    bShow = false;
  }
}

bool StoreData( void )
{
  //       0         1 2       3 4        5 6   7   8      9   1011
  //$GPRMC,hhmmss.ss,A,llll.ll,a,yyyyy.yy,a,x.x,x.x,ddmmyy,x.x,a*hh
  char cLat = gsGPSBuffer.charAt(giOffsets[3]);
  String sLatDeg = gsGPSBuffer.substring(giOffsets[2], giOffsets[2]+2);//first two numbers are the degrees
  int iLatDeg = sLatDeg.toInt();
  String sLatMin = gsGPSBuffer.substring(giOffsets[2]+2, giOffsets[3]-1);//mins with decimals

  char cLon = gsGPSBuffer.charAt(giOffsets[5]);
  String sLonDeg = gsGPSBuffer.substring(giOffsets[4], giOffsets[4]+3);
  int iLonDeg = sLonDeg.toInt();
  String sLonMin = gsGPSBuffer.substring(giOffsets[4]+3, giOffsets[5]-1);//mins with decimals

  bool bValid = true;

  bValid = iLatDeg > NORTHEST && iLatDeg < SOUTHEST;
  bValid = bValid && cLat == LATITUDE_CHAR;
  bValid = bValid && (iLonDeg < WESTEST && iLonDeg > EASTEST);
  bValid = bValid && cLon == LONGITUDE_CHAR;

  if(bValid)
  {
    //DSXXXX.XX,SYYYYY.YY
    gbNewData = true;
    gsBlock = "";
    gsBlock = (char)gbNewData;
    gsBlock += cLat;
    gsBlock += sLatDeg;
    gsBlock += sLatMin;
    gsBlock += ',';
    gsBlock += cLon;
    gsBlock += sLonDeg;
    gsBlock += sLonMin;
    gsBlock += '*'; //end of block
    bShow = true;
  }
  else
  {
    Serial.println("Data validation failed!");
    //ignore all readings...
  }
  
  return bValid;
}

bool ValidateBlock( void )
{
  bool bValid = gsGPSBuffer.length() == gsGPSBuffer.substring(giCheckSumPos).toInt();
  if(!bValid)
  {
    Serial.println("Checksum validation failed, ignoring...");
    bValid = true;
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
    Wire.write(gsBlock.c_str());
    gbNewData = false;
  }
}
