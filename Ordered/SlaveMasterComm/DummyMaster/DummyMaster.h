#include <Wire.h>

const int SLAVE_ADDRESS = 8;
const int DATA_LENGTH = //8char data twice plus 2 chars "sign" + 3 separating commas
sizeof(char)*2+
sizeof(int)*3*2;

String gsBuffer = "";

int		giLatDeg;
float	gfLatMin;
int		giLongDeg;
float	gfLongMin;
bool	gbNewData;

bool gbToogle = true;
