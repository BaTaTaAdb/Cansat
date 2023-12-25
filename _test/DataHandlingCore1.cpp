#include "DataHandlingCore1.h"
// Include necessary libraries
#include <SPI.h>
#include <SD.h>

void DataHandlingSetupCore1() {
  // Initialization for Core 1 (SD card, APC220)...
  

}

void Core1Loop() {
  while (true) {
    // Handle SD writing and APC220 communication...
  }
}

// Function to initialize SD card
bool initializeSD() {
    Serial.print("Initializing SD card...");

    // Check for SD card
    if (!SD.begin(chipSelect)) {
        Serial.println("initialization failed!");
        return false;
    }
    Serial.println("initialization done.");
    return true;
}

// Function to write data to SD card
int writeToSD(String fileName, double x, double y, double z, double headingVel, double descentVel, double pressure, double temperature) {
  File dataFile = SD.open(fileName.c_str(), FILE_WRITE);

  if (dataFile) {
    // Check if the file is empty and write the header
    if (dataFile.size() == 0) {
      dataFile.println("Timestamp,X-Position,Y-Position,Z-Position,Heading Velocity,Descent Velocity,Pressure,Temperature");
    }

    long unsigned timestamp = millis();
    dataFile.print(timestamp);
    dataFile.print(",");
    dataFile.print(x, 6);  // 6 decimal places for accuracy
    dataFile.print(",");
    dataFile.print(y, 6);
    dataFile.print(",");
    dataFile.print(z, 6);
    dataFile.print(",");
    dataFile.print(headingVel, 6);
    dataFile.print(",");
    dataFile.print(descentVel, 6);
    dataFile.print(",");
    dataFile.println(pressure, 6);
    dataFile.print(",");
    dataFile.print(temperature, 6);
    dataFile.close();
  } else {
    Serial.println("Error opening file for writing");
    return -1;
  }
  return 0;
}

String getNextFileName() {
  int fileNumber = 1;  // Start with file number 1
  String fileName;

  while (true) {
    fileName = "data" + String(fileNumber) + ".csv";
    if (!SD.exists(fileName.c_str())) {
      // File does not exist, we can use this name
      break;
    }
    fileNumber++;
  }

  return fileName;
}