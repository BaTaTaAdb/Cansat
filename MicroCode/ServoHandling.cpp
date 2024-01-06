#include "SharedData.h"
#include "ServoHandling.h"

#include <Arduino.h>
#include <Servo.h>

#define stop 90
#define delayValue 700
#define speed 90
#define headingObjective 77

// Define pulses for different directions
#define stopPulse 1500
#define clockwisePulse 1700         // Adjust as needed for clockwise rotation
#define counterClockwisePulse 1300  // Adjust as needed for counter-clockwise rotation

unsigned long moveDuration = 1000;  // duration for which the servo should move (in milliseconds)
unsigned long lastMoveTime = 0;     // time when the servo last started moving

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int directionToGo = 0;  //Direcao para onde o cansat tem de virar.
                        // 0 Se nao for para virar e -1 ou 1 otherwise (dependendo da direcao)
int magneticDirection = headingObjective;

void ServoSetup() {
  myservo.attach(pinServo);              // attaches the servo on GIO2 to the servo object
  myservo.writeMicroseconds(stopPulse);  // Initially stop the servo
}

void HandleInstructions() { 
  // Check if new data is ready
  if (receivedData.dataReady) {
    switch (receivedData.instruction) {
      case 1:
        myservo.writeMicroseconds(clockwisePulse);  // Rotate clockwise
        break;
      case -1:
        myservo.writeMicroseconds(counterClockwisePulse);  // Rotate counter-clockwise
        break;
      case 0:
      default:
        myservo.writeMicroseconds(stopPulse);  // Stop
        break;
    }
    receivedData.dataReady = false;  // Reset data flag
  }
}

void ServoAdjustDirection() {  // Ver se as condicoes iniciais estao de ==1 ou == -1 estao bem ou teem de ser trocadas. -----------------------------
  GetDirectionToGo();
  if (directionToGo == 1) {
    myservo.write(stop + speed);  // spin in one direction
    delay(delayValue);            // Ver se este tempo está certo
    myservo.write(stop - speed);  // spin in one direction
    delay(delayValue);            // Ver se este tempo está certo
    myservo.write(stop);
  } else if (directionToGo == -1) {
    myservo.write(stop - speed);  // spin in one direction
    delay(delayValue);            // Ver se este tempo está certo
    myservo.write(stop + speed);  // spin in one direction
    delay(delayValue);            // Ver se este tempo está certo
    myservo.write(stop);
  }
}

int GetDirectionToGo() {  // Verificar se o angulo e a direcao sao a mesma direcao ou n ----------------------------------
  if (servoData.dataReady) {
    // Copy the data
    ServoData localServoData = servoData;
    servoData.dataReady = false;  // Reset the flag
    magneticDirection = localServoData.heading;
    Serial.println(directionToGo);
  }
  if (abs(magneticDirection - headingObjective) <= threshold) {
    directionToGo = 0;
  } else {
    if (magneticDirection >= headingObjective && magneticDirection <= (headingObjective + 180)) {
      directionToGo = 1;
    } else {
      directionToGo = -1;
    }
    /*if (magneticDirection <= headingObjective || magneticDirection >= (headingObjective + 180)) {
      directionToGo = -1;
    }*/
  }
  return 0;
}
