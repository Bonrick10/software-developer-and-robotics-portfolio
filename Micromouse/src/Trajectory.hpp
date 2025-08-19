#pragma once

#include "config/config-motion.hpp"

namespace mtrn3100 {

class Trajectory {
public:
  Trajectory() {}
  Trajectory(float maxSpeed, float posA, float posB, float velA, float velB)
    : maxSpeed(maxSpeed), posA(posA), posB(posB), velA(velA), velB(velB) {}

  void tune(float maxSpeed, float posA, float posB, float velA, float velB) {
    this->maxSpeed = maxSpeed;
    this->posA = posA;
    this->posB = posB;
    this->velA = velA;
    this->velB = velB;
  }

  float compute(float currentPosition) {

    float t1 {0.5}, t2{};
    for (uint8_t i = 0; i < 5; i++) {
      t2 = t1 - (computePosition(t1, maxSpeed) - currentPosition) / computeVelocity(t1, maxSpeed);
      t1 = t2;
    }

    float v {computeVelocity(t2, maxSpeed)};
    if (v < MIN_MOTOR_V) v = MIN_MOTOR_V;
    return v;
  }

  float computePosition(float time, float maxSpeed) {
    return maxSpeed / PI * ((time - posA) * atan(posB * (time - posA)) -
           (log(pow(pow(posB, 2) * (time - posA), 2) + 1)) / (2 * posB)) +
           (MIN_MOTOR_V - maxSpeed / PI * atan(-posB * posA)) * time -
           maxSpeed / PI * (posA * atan(posB * posA) -
           log(pow(posB, 2) * pow(posA, 2) + 1) / (2 * posB));
  }

  float computeVelocity(float time, float maxSpeed) {
    return (maxSpeed / PI) * (atan(velB * (time - velA)))
           + MIN_MOTOR_V - maxSpeed / PI * atan(-velB * velA);
  }

private:
  float maxSpeed{};
  float posA{}, posB{};
  float velA{}, velB{};
};

} // namespace mtrn3100