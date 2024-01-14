#ifndef SERVOHANDLING_H
#define SERVOHANDLING_H

#define threshold 7.5
#define pinServo 14

void ServoSetup();
void ServoTest();
void ServoTestSimple();
int GetDirectionToGo();
void ServoAdjustDirection();
void HandleInstructions();
#endif
