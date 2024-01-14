#include "SensorHandling.h"
#include "SharedData.h"

// Include sensor libraries
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_BME280.h>
#include <Adafruit_GPS.h>
#include <utility/imumaths.h>

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

/* Define the DECLINATION_ANGLE */
#define DECLINATION_ANGLE -1.27

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);
Adafruit_BME280 bme;  // I2C

/* Defining GPS Serial */
#define GPSSerial Serial1
Adafruit_GPS GPS(&GPSSerial);
#define GPSECHO false

unsigned int prevMillis = 0;

int SensorSetup() {
  // Initialize BNO055
  /*
  if (!bno.begin()) {
    Serial.println("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    return -1;
  };

  // Initialize BME280
  if (!bme.begin()) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    return -1;
  }

  delay(1000);

  // Set the mode to NDOF
  bno.setMode(adafruit_bno055_opmode_t::OPERATION_MODE_COMPASS);


  if (sensorDebug) {
    Serial.println("Orientation Sensor Test");
    Serial.println("");
    // Display some basic information on this sensor 
    displaySensorDetails();

    // Optional: Display current status 
    displaySensorStatus();
  }
  bno.setExtCrystalUse(true);

  // Initialize GPS
  // 9600 NMEA is the default baud rate for Adafruit GPS
  if (!GPS.begin(9600)) {
    Serial.println("Could not find a GPS, check wiring!");
    return -1;
  }

  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);

  delay(1000);

  // Ask for GPS firmware version
  GPSSerial.println(PMTK_Q_RELEASE);

  */
  return 0;
}

void HandleSensors() {
  if ((millis() - prevMillis) <= BNO055_SAMPLERATE_DELAY_MS) return;
  /*while (!bno.isFullyCalibrated()) {
    displayCalStatus();
  }*/
  // Get a new sensor event
  /*
  sensors_event_t event;
  bno.getEvent(&event);
  */

  // Get temperature and  dpressureata from bme280
  /*
  float temperatureAmbient = bme.readTemperature();
  float pressure = bme.readPressure() / 100;
  float altitude = bme.readAltitude(seaLevelPressure_hPa);
  float temperatureCPU = analogReadTemp();
  */

  // Display the floating point data
  /*
  if (sensorDebug) {
    Serial.printf("\tCompass: %.4f", event.orientation.x);
    Serial.printf("\tAmbient Temperature: %.2f°C", temperatureAmbient);
    Serial.printf("\tCore temperature: %2.1f°C", temperatureCPU);
    Serial.printf("\tPressure: %.4fHBar", pressure);

    // New line for the next sample 
    Serial.println("\t");
  }
  */
  // Populate sharedData
  /*
  sharedData.compass = event.orientation.x;
  sharedData.altitude = altitude;
  sharedData.pressure = pressure;
  sharedData.temperatureAmbient = temperatureAmbient;
  sharedData.temperatureCPU = temperatureCPU;
  sharedData.dataReady = true;  // Indicate that new data is ready
  */
  float temperatureCPU = analogReadTemp();
  sharedData.compass = 0;
  sharedData.altitude = 0;
  sharedData.pressure = 0;
  sharedData.temperatureAmbient = 0;
  sharedData.temperatureCPU = temperatureCPU;
  sharedData.dataReady = true;  // Indicate that new data is ready

  /* Optional: Display calibration status */
  //displayCalStatus();

  /* Optional: Display sensor status (debug only) */
  //displaySensorStatus();


  /* Wait the specified delay before requesting nex data */
  // delay(BNO055_SAMPLERATE_DELAY_MS);
  prevMillis = millis();
}

/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/
void displaySensorDetails(void) {
  sensor_t sensor;
  bno.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print("Sensor:       ");
  Serial.println(sensor.name);
  Serial.print("Driver Ver:   ");
  Serial.println(sensor.version);
  Serial.print("Unique ID:    ");
  Serial.println(sensor.sensor_id);
  Serial.print("Max Value:    ");
  Serial.print(sensor.max_value);
  Serial.println(" xxx");
  Serial.print("Min Value:    ");
  Serial.print(sensor.min_value);
  Serial.println(" xxx");
  Serial.print("Resolution:   ");
  Serial.print(sensor.resolution);
  Serial.println(" xxx");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(200);
}

/**************************************************************************/
/*
    Display some basic info about the sensor status
*/
/**************************************************************************/
void displaySensorStatus(void) {
  /* Get the system status values (mostly for debugging purposes) */
  uint8_t system_status, self_test_results, system_error;
  system_status = self_test_results = system_error = 0;
  bno.getSystemStatus(&system_status, &self_test_results, &system_error);

  /* Display the results in the Serial Monitor */
  Serial.println("");
  Serial.print("System Status: 0x");
  Serial.println(system_status, HEX);
  Serial.print("Self Test:     0x");
  Serial.println(self_test_results, HEX);
  Serial.print("System Error:  0x");
  Serial.println(system_error, HEX);
  Serial.println("");
  delay(200);
}

// Displays sensor calibration status
void displayCalStatus(void) {
  /* Get the four calibration values (0..3) */
  /* Any sensor data reporting 0 should be ignored, */
  /* 3 means 'fully calibrated" */
  uint8_t system, gyro, accel, mag;
  system = gyro = accel = mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);

  /* The data should be ignored until the system calibration is > 0 */
  Serial.print("\t");
  if (!system) {
    Serial.print("! ");
  }

  /* Display the individual values */
  Serial.print("Sys:");
  Serial.print(system, DEC);
  Serial.print(" G:");
  Serial.print(gyro, DEC);
  Serial.print(" A:");
  Serial.print(accel, DEC);
  Serial.print(" M:");
  Serial.print(mag, DEC);
  Serial.println("");
}