#include <Wire.h>

const int SLAVE_ADDRESS = 8;
const int DATA_LENGTH = 9;

String gsBuffer = "";

int		giLatDeg;
float	gfLatMin;
int		giLongDeg;
float	gfLongMin;
bool	gbNewData;
