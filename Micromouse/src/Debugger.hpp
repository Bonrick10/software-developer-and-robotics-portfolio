#pragma once

#include "OLEDDisplay.hpp"
#include "EncoderOdometry.hpp"
#include "IMU.hpp"
#include "DualEncoder.hpp"
#include "LiDARSensor.hpp"

namespace mtrn3100 {

class Debugger {
public:
  Debugger(OLEDDisplay &display, EncoderOdometry &odom, IMU &imu, DualEncoder &enc, LiDARSensor &sensorL, LiDARSensor &sensorF, LiDARSensor &sensorR)
    : display(display), odom(odom), imu(imu), enc(enc), sensorL(sensorL), sensorF(sensorF), sensorR(sensorR) {}

  void displayLiDARs() const {
    display.setCursor(48, 0);
    char buffer[4];
    sprintf(buffer, "%03d", static_cast<int>(sensorF.readDistance()));
    display.println(buffer);

    display.setCursor(0, 2);
    sprintf(buffer, "%03d", static_cast<int>(sensorL.readDistance()));
    display.print(buffer);

    display.setCursor(90, 2);
    sprintf(buffer, "%03d", static_cast<int>(sensorR.readDistance()));
    display.print(buffer);
  }


  void displayEncoder() const {
    display.clear();
    display.setCursor(0, 0);
    display.print(static_cast<int>(enc.getLeftRotation()));
    display.setCursor(90, 0);
    display.print(static_cast<int>(enc.getRightRotation()));
  }

  void displayOdom() const {
    display.clear();
    display.setCursor(48, 0);
    display.println(static_cast<int>(imu.getAngleZCompensated()));
    display.setCursor(0, 2);
    display.print(static_cast<int>(odom.getX()));
    display.setCursor(90, 2);
    display.print(static_cast<int>(odom.getY()));
  }

  void displayIMU() const {
    display.clear();
    display.setCursor(48, 0);
    display.println(static_cast<int>(imu.getAngleZCompensated()));
  }

protected:
  OLEDDisplay &display;
  EncoderOdometry &odom;
  IMU &imu;
  DualEncoder &enc;
  LiDARSensor &sensorL;
  LiDARSensor &sensorF;
  LiDARSensor &sensorR;
};

} // namespace mtrn3100
