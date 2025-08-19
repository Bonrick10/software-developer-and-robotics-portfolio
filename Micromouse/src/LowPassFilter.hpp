#pragma once

namespace mtrn3100 {

class LowPassFilter {
public:
  LowPassFilter(float alpha) : alpha(alpha), prevValue(0) {}

  float filter(float value) {
    prevValue = alpha * value + (1 - alpha) * prevValue;
    return prevValue;
  }

  static float varFilter(float alpha, float prevValue, float value) {
    prevValue = alpha * value + (1 - alpha) * prevValue;
    return prevValue;
  }

private:
  float alpha; // Smoothing factor (0 < alpha < 1)
  float prevValue;
};

}
