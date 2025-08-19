#pragma once

#include <VL6180X.h>
#include <Wire.h>
#include "LowPassFilter.hpp"

namespace mtrn3100 {

class LiDARSensor {
public:
  LiDARSensor(uint8_t address, uint8_t enablePin)
    : address(address), enablePin(enablePin), slowFilter(0.1), fastFilter(0.8), prevDist(0) {
    pinMode(enablePin, OUTPUT);
    digitalWrite(enablePin, LOW); // Start with the sensor disabled
  }

  bool begin() {
    digitalWrite(enablePin, HIGH); // Enable the sensor
    delay(50); // Allow some time for the sensor to power up

    sensor.init();
    sensor.configureDefault();
    sensor.setTimeout(250); // Set a 250ms timeout for sensor operations
    sensor.setAddress(address); // Set the new I2C address
    delay(100);

    // Perform a test read to ensure the sensor is working
    uint8_t distance = sensor.readRangeSingleMillimeters();
    if (sensor.timeoutOccurred()) {
      Serial.print("Failed to init sensor at address 0x");
      Serial.println(address, HEX);
      digitalWrite(enablePin, LOW); // Disable the sensor if initialization fails
      return false; // Initialization failed
    }

    Serial.print("Sensor initialized at address 0x");
    Serial.println(address, HEX);
    return true; // Initialization successful
  }

  uint8_t readDistance() {
    prevDist = LowPassFilter::varFilter(0.8, prevDist, sensor.readRangeSingleMillimeters());
    return prevDist;
  }

  bool timeoutOccurred() {
    return sensor.timeoutOccurred();
  }

  void disableSensor() {
    digitalWrite(enablePin, LOW);
  }

  float readBandPassDistance() {
    float rawDistance = sensor.readRangeSingleMillimeters();
    float slowFiltered = slowFilter.filter(rawDistance);
    float fastFiltered = fastFilter.filter(rawDistance);
    return slowFiltered - fastFiltered;
  }

private:
  VL6180X sensor;
  uint8_t address;
  uint8_t enablePin;
  float prevDist;

  LowPassFilter slowFilter;
  LowPassFilter fastFilter;
};

} // namespace mtrn3100
