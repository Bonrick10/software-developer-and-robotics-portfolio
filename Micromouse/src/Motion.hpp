#pragma once

#include "EncoderOdometry.hpp"
#include "DualEncoder.hpp"
#include "PIDController.hpp"
#include "Motor.hpp"
#include "IMU.hpp"
#include "LiDARSensor.hpp"
#include <math.h>
#include "config/config-motion.hpp"
#include "OLEDDisplay.hpp"
#include "Trajectory.hpp"
#include "Debugger.hpp"

namespace mtrn3100 {

class Motion {
public:
  Motion(float r, float b,
         Motor &motorLeft, Motor &motorRight,
         DualEncoder &enc, IMU &imu,
         LiDARSensor &sensorL, LiDARSensor &sensorF, LiDARSensor &sensorR,
         OLEDDisplay &display)
  : r(r), b(b),
    v(), w(),
    leftWallPresent(true), rightWallPresent(true),
    motorLeft(motorLeft),
    motorRight(motorRight),
    enc(enc),
    imu(imu),
    sensorL(sensorL), sensorF(sensorF), sensorR(sensorR),
    display(display),
    odom(r, b/2),
    traj(),
    controllerMoveDist(),
    controllerRotateAngle(),
    controllerIMUAdjust(),
    controllerLiDARAdjust(),
    controllerFrontLiDAR(),
    debugger(display, odom, imu, enc, sensorL, sensorF, sensorR) {}

  void setup() {
    imu.setDriftRate(1/1e9);

    float currIMUAngleZ {imu.getAngleZCompensated()};

    traj.tune(200, 5, 3, 3, 5);

    controllerMoveDist.zeroAndSetTarget(0, 0);
    controllerMoveDist.tune(0.7, 0, 0.02);
    controllerMoveDist.setMinOutput(MIN_MOTOR_V);
    controllerMoveDist.setDeadband(0);

    controllerRotateAngle.zeroAndSetTarget(currIMUAngleZ, 0);
    controllerRotateAngle.tune(0.016, 0, 0);
    controllerRotateAngle.setMinOutput(MIN_MOTOR_W);
    controllerRotateAngle.setDeadband(0.05);

    controllerIMUAdjust.zeroAndSetTarget(currIMUAngleZ, 0);
    controllerIMUAdjust.tune(0.05, 0, 0);
    controllerIMUAdjust.setDeadband(0);

    controllerLiDARAdjust.zeroAndSetTarget(IDEAL_LIDAR_DIST, 0);
    controllerLiDARAdjust.tune(0.02, 0, 0);
    controllerLiDARAdjust.setDeadband(0);

    controllerFrontLiDAR.zeroAndSetTarget(MIN_FRONT_DIST, 0);
    controllerFrontLiDAR.tune(1, 0, 0);
  }

  void update() {
    odom.update(enc.getLeftRotation(), enc.getRightRotation());

    // if (leftWallPresent && detectLeftWallMissing())
    //   leftWallPresent = false;
    // else if (!leftWallPresent && detectLeftWallAppearing())
    //   leftWallPresent = true;

    // if (rightWallPresent && detectRightWallMissing())
    //   rightWallPresent = false;
    // else if (!rightWallPresent && detectRightWallAppearing())
    //   rightWallPresent = true;

    // prevLeftBPDist = sensorL.readBandPassDistance();
    // prevRightBPDist = sensorR.readBandPassDistance();
  }

  void setMotion(float v_ref, float w_ref) {
    motorLeft.setPWM(angularVelocityToPWM(v_ref / r - b * w_ref / (2 * r)));
    motorRight.setPWM(angularVelocityToPWM(v_ref / r + b * w_ref / (2 * r)));
  }

  bool moveDistance(float distance, float refX, float refY, float refH) {
    controllerMoveDist.updateTarget(distance);
    controllerIMUAdjust.updateTarget(refH);

    float relPos {sqrt(pow(odom.getX() - refX, 2)+pow(odom.getY() - refY, 2))};

    if (frontStoppingDist()) {
      // Slows down mouse to MIN_FRONT_DIST of front LiDAR
      v = -controllerFrontLiDAR.compute(sensorF.readDistance());
      w = 0;
    } else {
      // Accerlerate mouse at the first 50% distance with Cubic Trajectory,
      // Decelerate mouse at the last 50% distance with PID
      v = relPos < distance * 0.7 ?
          traj.compute(relPos) : controllerMoveDist.compute(relPos);
      // Uses IMU adjust if the mouse is within IMU_ACTIVE_DIST from centre,
      // otherwise uses LiDAR adjust
      w = fabs(IDEAL_LIDAR_DIST - sensorL.readDistance()) < IMU_ACTIVE_DIST ?
          angularVelocityWeight(v) * controllerIMUAdjust.compute(imu.getAngleZCompensated()) :
          -angularVelocityWeight(v) * controllerLiDARAdjust.compute(sensorL.readDistance());
    }

    setMotion(v, w);
    if (fabs(distance - relPos) < 1) return true;

    return false;
  }

  bool moveAngle(float targetH, float refH) {
    controllerRotateAngle.updateZeroAndTarget(refH, targetH);
    v = 0;
    w = controllerRotateAngle.compute(imu.getAngleZCompensated());
    setMotion(v, constrain(w, -0.7, 0.7));
    if (w == 0) return true;

    return false;
  }

  float getCurrOdomX() const { return odom.getX(); }
  float getCurrOdomY() const { return odom.getY(); }
  float getCurrOdomDeg() const { return odom.getH(); }
  float getCurrIMUDeg() const { return imu.getAngleZCompensated(); }
  float getLeftDist() const { return sensorL.readDistance(); }
  float getFrontDist() const { return sensorF.readDistance(); }
  float getRightDist() const { return sensorR.readDistance(); }
  float getLeftBPDist() const { return sensorL.readBandPassDistance(); }
  float getFrontBPDist() const { return sensorF.readBandPassDistance(); }
  float getRightBPDist() const { return sensorR.readBandPassDistance(); }

private:
  float angularVelocityToPWM(float desiredAngularVelocity) {
    return constrain(W2PWM * desiredAngularVelocity, -MAX_PWM, MAX_PWM);
  }

  float angularVelocityWeight(float v) {
    return exp(-0.9 * (1 - (v - MIN_MOTOR_V) / (MAX_MOTOR_V - MIN_MOTOR_V)));
  }

  // bool detectLeftWallMissing() {
  //   return prevLeftBPDist - sensorL.readBandPassDistance() > 20;
  // }

  // bool detectRightWallMissing() {
  //   return prevRightBPDist - sensorR.readBandPassDistance() > 20;
  // }

  // bool detectLeftWallAppearing() {
  //   return prevLeftBPDist - sensorL.readBandPassDistance() < -20;
  // }

  // bool detectRightWallAppearing() {
  //   return prevRightBPDist - sensorR.readBandPassDistance() < -20;
  // }

  bool frontStoppingDist() {
    return sensorF.readDistance() < FRONT_DIST_SLOW_DOWN;
  }

public: // members
  Debugger debugger;

private: // members
  float r{}, b{};
  float v{}, w{};

  bool leftWallPresent{}, rightWallPresent{};

  Motor &motorLeft;
  Motor &motorRight;
  DualEncoder &enc;
  IMU &imu;
  LiDARSensor &sensorL, &sensorF, &sensorR;
  OLEDDisplay &display;

  EncoderOdometry odom;

  Trajectory traj;

  PIDController controllerMoveDist;
  PIDController controllerRotateAngle;
  PIDController controllerIMUAdjust;
  PIDController controllerLiDARAdjust;
  PIDController controllerFrontLiDAR;
};

} // namespace mtrn3100
