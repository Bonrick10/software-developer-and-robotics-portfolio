#pragma once
// MicroMouse Dimensions for Odometry
constexpr float WHEEL_RADIUS {15.73};
constexpr float BASE_LENGTH {118};

// LiDAR Sensor Pins and Addresses
constexpr uint8_t SENSOR1_PIN {A0};
constexpr uint8_t SENSOR2_PIN {A1};
constexpr uint8_t SENSOR3_PIN {A2};
constexpr uint8_t SENSOR1_ADDRESS {0x30};
constexpr uint8_t SENSOR2_ADDRESS {0x32};
constexpr uint8_t SENSOR3_ADDRESS {0x34};

// OLED Display Address
constexpr uint8_t SCREEN_ADDRESS {0x3C};

// motor 1 pins (left motor)
constexpr uint8_t MOT1ENCA {2};
constexpr uint8_t MOT1ENCB {7};
constexpr uint8_t MOT1PWM  {11};
constexpr uint8_t MOT1DIR  {12};

// motor 2 pins (right motor)
constexpr uint8_t MOT2ENCA {3};
constexpr uint8_t MOT2ENCB {8};
constexpr uint8_t MOT2PWM  {9};
constexpr uint8_t MOT2DIR  {10};
