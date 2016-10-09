String gcBuffer[100] = NULL;
const String gsSentence = "$GPRCM";
String gcOffsets[12] = NULL;
unsigned int giChecksumLength = 0;
unsigned int giChecksumOffset = 0;

void Step_ValidateWaypoint( void )
{
  //"$GPRMC,220516,A,5133.82,N,00042.24,W,173.8,231.8,130694,004.2,W*70"
  gcBuffer[0] = '$'; //if we got this far we already have this one
  gbValidWaypoint = true;

  gbValidWaypoint = ValidateSentence();
  gbValidWaypoint = gbValidWaypoint && ReadAllStream();
  gbValidWaypoint = gbValidWaypoint && ValidateData();
}

bool ValidateData( void )
{
  bool bValid = false;
  int iBufferLength = GetBufferLength();
  String cCheckSum[giChecksumLength];
  
  //get the checksum from the stream as a int to compare
  memcpy(cCheckSum, gcBuffer[giChecksumOffset], giChecksumLength);
  int iCheckSum = cCheckSum.toInt();
  if( iCheckSum == iBufferLength )
  {
    bValid = true;
  }
  return bValid;
}

int GetBufferLength( void )
{
  int i = 0;
  
  //we do not validate buffer overflow
  //at this instance it should be alredy been validated
  while( gcBuffer[i] != ';' )
  {
    i++;
  }
  return i;
}

bool ReadAllStream( void )
{
  char cLatestData = -1;
  unsigned int iDataOffset = 0;
  int i = 0;
  
  while( 10 != cLatestData && i < 100)
  {
    cLatestData = Serial.read();
    if( cLatestData == ',')
    {
      //store the offsets to identify each data block later
      gcOffsets[iDataOffset++] = i+1;//+1 for we already have the '$'
    }
    if( cLatestData == '*')
    {
      //store the offsets for the checksum
      giChecksumOffset = i+1;//+1 for we already have the '$'
    }
    if(cLatestData == 13)
    {
      gcChecksumLength = i - giChecksumOffset;
    }

    gcBuffer[i+1] = cLatestData; //+1 for we already have the '$'
    i++;
  }
  if( i > MAX_BUFFER_LENGTH )
  {
    //this is to prevent buffer overflow
    //if something went wrong just ignore tha package and star over
    return false;
  }
}

bool ValidateSentence( void )
{
  //won't validate first one for it took us as this far already
  for(int i = 1; i < 6; i++)
  {
    gcBuffer[i] = Serial.read();
    if(gcBuffer[i] != gcSentence[i])
    {
      return false;
    }
  }

  return true;
}

