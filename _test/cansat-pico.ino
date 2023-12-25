#include "SensorHandling.h"
#include "ServoHandling.h"

void setup() {
  SensorSetup();
  ServoSetup();
}

void loop() {
  HandleSensors();
  ServoTest();
}
