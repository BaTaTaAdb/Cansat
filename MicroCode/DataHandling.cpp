#include "DataHandling.h"
#include "SharedData.h"

// Include necessary libraries
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

int led = LED_BUILTIN;
unsigned long timeSent = 0;
String fileName = "";

int DataHandlingSetup() {
  pinMode(led, OUTPUT);
  // Initialization for Core 1 (SD card, APC220)
  // Initialize SD card
  if (!initializeSD()) return -1;

  // Initialize APC220
  Serial2.begin(9600);

  while (!Serial2)
    ;
  Serial.println("APC220 is ready to use.");
  Serial2.println("APC220: Hello World!");

  fileName = getNextFileName();  // Stores new file name
  return 0;
}

void HandleData() {
  if (Serial2.available()) {
    // Read incoming instrucions (1=right, -1=left, 0=straight)
    int incomingInstruction = Serial2.parseInt();

    if (!(incomingInstruction == -1 || incomingInstruction == 0 || incomingInstruction == 1)) {
      incomingInstruction = 0;
      Serial.println("Error parsing received instruction!!!");
    } else {
      // Process the byte (for now, just print it)
      Serial.print("Received: ");
      Serial.println(incomingInstruction);
    }
    receivedData.instruction = incomingInstruction;
    receivedData.dataReady = true;
  }

  if (!sharedData.dataReady) return;
  if (timeSent + sendDelay > millis()) return;
  digitalWrite(led, HIGH);
  // Copy the data
  SharedData localData = sharedData;
  sharedData.dataReady = false;  // Reset the flag

  // Handle SD card writing
  //writeToSD(fileName, localData.compass, localData.pressure, localData.temperatureAmbient, localData.temperatureCPU, localData.altitude);

  // Handle APC220 transmission
  // Format the data into a string or binary format and send over APC220
  // Sends new data
  Serial2.print(localData.compass);
  Serial2.print(",");
  Serial2.print(localData.pressure);
  Serial2.print(",");
  Serial2.print(localData.temperatureAmbient);
  Serial2.print(",");
  Serial2.print(localData.temperatureCPU);
  Serial2.print(",");
  Serial2.println(localData.altitude);

  digitalWrite(led, LOW);

  servoData.heading = localData.compass;
  servoData.dataReady = true;

  timeSent = millis();
}

// Function to initialize SD card
bool initializeSD() {
  Serial.println("Initializing SD card...");

  SPI.begin();  // Initialize SPI1
  // Check for SD card
  if (!SD.begin(chipSelect)) {
    Serial.println("SD Card initialization failed!");
    return false;
  }
  Serial.print("initialization done.\n");
  return true;
}

// Function to write data to SD card
int writeToSD(String fileName, double compass, double pressure, double temperatureAmbient, double temperatureCPU, double altitude) {
  File dataFile = SD.open(fileName.c_str(), FILE_WRITE);

  if (dataFile) {
    // Check if the file is empty and write the header
    if (dataFile.size() == 0) {
      dataFile.println("timestamp,compass,pressure,temperatureAmbient,temperatureCPU,altitude");
    }

    long unsigned timestamp = millis();
    dataFile.print(timestamp);
    dataFile.print(",");
    dataFile.print(compass, 6);  // 6 decimal places for accuracy
    dataFile.print(",");
    dataFile.print(pressure, 6);
    dataFile.print(",");
    dataFile.print(temperatureAmbient, 6);
    dataFile.print(",");
    dataFile.print(temperatureCPU, 6);
    dataFile.print(",");
    dataFile.println(altitude, 6);
  } else {
    Serial.println("Error opening file for writing");
    return -1;
  }
  return 0;
}

String getNextFileName() {
  int fileNumber = 1;  // Start with file number 1
  char fileName[20];   // Buffer to hold the file name

  while (true) {
    sprintf(fileName, "data%04d.csv", fileNumber);  // Format with leading zeros
    if (!SD.exists(fileName)) {
      // File does not exist, we can use this name
      break;
    }
    fileNumber++;
  }

  return fileName;
}