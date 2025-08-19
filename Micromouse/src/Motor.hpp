#pragma once

#include <Arduino.h>

#include "math.h"

namespace mtrn3100 {

class Motor {
public:
  Motor( uint8_t pwm_pin, uint8_t in2) : pwm_pin(pwm_pin), dir_pin(in2) {
    pinMode(pwm_pin, OUTPUT);
    pinMode(dir_pin, OUTPUT);
  }

  void setPWM(int16_t pwm) {
    if (pwm > 0) {
      digitalWrite(dir_pin, HIGH);
      analogWrite(pwm_pin, pwm);
    } else {
      digitalWrite(dir_pin, LOW);
      analogWrite(pwm_pin, -pwm);
    }
  }

private:
  const uint8_t pwm_pin;
  const uint8_t dir_pin;
};

}  // namespace mtrn3100
