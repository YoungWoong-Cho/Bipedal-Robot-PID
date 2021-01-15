#include "BipedalPID.h"

BipedalPID obj;

void setup(){Serial.begin(9600);  }
void loop(){obj.run();}
