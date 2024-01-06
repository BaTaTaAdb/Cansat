#ifndef SENSORHANDLING_H
#define SENSORHANDLING_H

#define seaLevelPressure_hPa 1032.65
#define INTERRUPT_PIN 5
#define sensorDebug false
#include <Arduino.h>

int SensorSetup();
void HandleSensors();
void displaySensorStatus();
void displayCalStatus();
void displaySensorDetails();
void writeBNO055Register(byte reg, byte value);

#endif