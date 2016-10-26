#include <Wire.h>

const int SLAVE_ADDRESS = 8;
const int DATA_LENGTH = 8*2+2+3;//8char data twice plus 2 chars "sign" + 3 separating commas
String gsBuffer = "";