#include "BipedalPID.h"

BipedalPID* robot;

void setup(){robot = new BipedalPID(0, 0, 0);}
void loop(){
  robot->run();
}
