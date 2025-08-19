#pragma once

#include <Arduino.h>

namespace mtrn3100 {
  class EncoderOdometry {
  public:
		// Constructor: radius (R) in millimeters, wheelBase (L) in millimeters
    EncoderOdometry(float radius, float wheelBase)
    : x(0), y(0), h(0), R(radius), L(wheelBase), lastLPos(0), lastRPos(0) {}

    void update(float leftValue, float rightValue) {

      float delta_left_radians = leftValue - lastLPos;
      float delta_right_radians = rightValue - lastRPos;

      float deltaS = R / 2 * (delta_left_radians + delta_right_radians);
      x += deltaS * cos(h);
      y += deltaS * sin(h);
      h += R / (2 * L) * (delta_right_radians - delta_left_radians);

      lastLPos = leftValue;
      lastRPos = rightValue;
    }

    float getX() const { return x; }
    float getY() const { return y; }
    float getH() const { return h; }
    float getHDeg() const { return h * 180 / PI; }

  private:
    float x, y, h;
    const float R, L;
    float lastLPos, lastRPos;
  };
}
