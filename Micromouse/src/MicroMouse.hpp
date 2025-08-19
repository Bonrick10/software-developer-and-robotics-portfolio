#pragma once
#include <Wire.h>
#include "EncoderOdometry.hpp"
#include "DualEncoder.hpp"
#include "PIDController.hpp"
#include "Motor.hpp"
#include "IMU.hpp"
#include "OLEDDisplay.hpp"
#include "LiDARSensor.hpp"
#include "Motion.hpp"
#include "StringCommand.hpp"

namespace mtrn3100 {

// Define the freeMemory function
extern int __heap_start, *__brkval;
int freeMemory() {
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

class MicroMouse {
public:
  MicroMouse(
    TwoWire &w,
    const uint8_t SENSORL_PIN, const uint8_t SENSORL_ADDRESS,
    const uint8_t SENSORF_PIN, const uint8_t SENSORF_ADDRESS,
    const uint8_t SENSORR_PIN, const uint8_t SENSORR_ADDRESS,
    const uint8_t SCREEN_ADDRESS,
    const uint8_t MOT1PWM_PIN, const uint8_t MOT1DIR_PIN,
    const uint8_t MOT2PWM_PIN, const uint8_t MOT2DIR_PIN,
    const uint8_t MOT1ENCA_PIN, const uint8_t MOT1ENCB_PIN,
    const uint8_t MOT2ENCA_PIN, const uint8_t MOT2ENCB_PIN,
    float r, float b
  ) : imu(w),
      sensorL(SENSORL_ADDRESS, SENSORL_PIN),
      sensorF(SENSORF_ADDRESS, SENSORF_PIN),
      sensorR(SENSORR_ADDRESS, SENSORR_PIN),
      display(SCREEN_ADDRESS),
      motorLeft(MOT1PWM_PIN, MOT1DIR_PIN),
      motorRight(MOT2PWM_PIN, MOT2DIR_PIN),
      encoders(MOT1ENCA_PIN, MOT1ENCB_PIN, MOT2ENCA_PIN, MOT2ENCB_PIN),
      motion(r, b, motorLeft, motorRight, encoders, imu, sensorL, sensorF, sensorR, display),
      stringCommand(motion, STR_CMD) {}

  void setup() {
    // Setup OLED Display
    display.setup();

    display.clear();
    display.set2X();
    display.println(F("Setting Up"));
    delay(1000);

    display.clear();
    display.println(F("DON'T MOVE"));

    imu.setup();

    // Setup LiDAR Sensors
    byte sensorLStatus {sensorL.begin()};
    Serial.print(F("VL6180X A2 Status: "));
    Serial.println(sensorLStatus);
    while(!sensorLStatus){ }
    byte sensorFStatus {sensorF.begin()};
    Serial.print(F("VL6180X A3 Status: "));
    Serial.println(sensorFStatus);
    while(!sensorFStatus){ }
    byte sensorRStatus {sensorR.begin()};
    Serial.print(F("VL6180X A4 Status: "));
    Serial.println(sensorRStatus);
    while(!sensorRStatus){ }

    motion.setup();

    // take 10 updates for band-pass filters to stabilise
    // for (int i{}; i < 10; i++) {
    //   update();
    //   delay(1);
    // }

    display.clear();
    display.println(F("Ready"));
		delay(2000);
    display.clear();
  }

  void update() {
    imu.update();
    motion.update();
  }

  void run() {
    update();
    freeMemory();
    stringCommand.run();
    motion.debugger.displayLiDARs();
  }

private:
  // Components
  IMU imu;
  LiDARSensor sensorL;
  LiDARSensor sensorF;
  LiDARSensor sensorR;
  Motor motorLeft;
  Motor motorRight;
  DualEncoder encoders;
  OLEDDisplay display;

  Motion motion;
  StringCommand stringCommand;
};

} // namespace mtrn3100
