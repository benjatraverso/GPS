void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  gsBuffer.reserve(50);
}

void loop()
{
  Wire.requestFrom(SLAVE_ADDRESS, DATA_LENGTH);    // request 6 bytes from slave device #8

  int deg = Wire.read();
  while( Wire.available() )
  {
    gsBuffer =+ Wire.read(); // receive a byte as character
  }

  printBuffer();
  gsBuffer = "";

  delay(1000);
}

void printBuffer(void)
{
	Serial.println(gsBuffer);
}