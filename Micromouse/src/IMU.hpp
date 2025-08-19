#pragma once

#include <Arduino.h>
#include <MPU6050_light.h>
#include <Wire.h>
#include "LowPassFilter.hpp"

namespace mtrn3100 {

class IMU : public MPU6050 {
public:
  IMU(TwoWire &w, float driftRate=0) : MPU6050(w), driftRate(driftRate) {}

  void setup(){
    byte mpuStatus {begin()};
    Serial.print(F("MPU6050 status: "));
    Serial.println(!mpuStatus); // success if mpuStatus==0
    while(mpuStatus!=0){ } // stop everything if could not connect to MPU6050
    Serial.println(F("Calculating offsets, do not move MPU6050"));
    delay(1000);
    calcOffsets(true,true);
    startTime = micros();
    Serial.println("Done!\n");
  }

  void reset() {
    // Reset the MPU6050
    writeData(MPU6050_PWR_MGMT_1_REGISTER, 0x80); // Write a 1 to bit 7 to reset the MPU6050
    delay(100); // Wait for the reset to complete
  }

  void setDriftRate(float dr) {
    driftRate = dr;
  }

  float getAngleZCompensated() {
    return getAngleZ() - (micros() - startTime) * driftRate;
  }

private:
  float startTime{};
  float driftRate{};
};

} // namespace mtrn3100
