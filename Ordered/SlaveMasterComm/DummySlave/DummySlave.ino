#include <Wire.h>

bool gbNewData = false;

void setup()
{
  Wire.begin(8);
  Wire.onRequest(requestEvent);
}

void loop()
{
  gbNewData = !gbNewData;//depending on when the request falls, we send new data or not
  delay(2000);
}


void requestEvent( void )
{
  Wire.write(gbNewData); //always indicate if we have new data
  if(gbNewData)
  {
    //XllllllYyyyyyyy
    //S363382W0604224
    Wire.write('S');
    Wire.write(36);
    Wire.write(33);
    Wire.write(82);
  
    Wire.write('W');
    Wire.write((int)060);
    Wire.write(42);
    Wire.write(24);
  }
}
