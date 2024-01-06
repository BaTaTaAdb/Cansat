#ifndef SHARED_DATA_H
#define SHARED_DATA_H

struct SharedData {
  double compass, pressure, temperatureAmbient, temperatureCPU, altitude;
  volatile bool dataReady;  // Flag to indicate new data is ready
};

struct ServoData{
  double heading;
  volatile bool dataReady; // Flag to indicate new data is ready
};

struct ReceivedData {
  int instruction;
  volatile bool dataReady; // Flag to indicate new data is ready
};

extern SharedData sharedData;  // Declaration of the shared data object
extern ServoData servoData;  // Declaration of the shared data object
extern ReceivedData receivedData;  // Declaration of the received data object

#endif