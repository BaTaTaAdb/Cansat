#include "SensorHandling.h"
#include "ServoHandling.h"
#include "DataHandling.h"
#include "SharedData.h"

SharedData sharedData;  // Define the shared data object
ServoData servoData;    // Define the servo data object
ReceivedData receivedData; // Define the received data object


void setup() {
  Serial.begin(9600);
  DataHandlingSetup();
  SensorSetup();
}

void loop() {
  HandleSensors();
  HandleData();
}

void setup1() {
  delay(1000);
  ServoSetup();
}

void loop1() {
  HandleInstructions();
}