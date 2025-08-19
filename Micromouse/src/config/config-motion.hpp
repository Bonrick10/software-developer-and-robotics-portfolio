#pragma once
constexpr float MIN_MOTOR_V {12};
constexpr float MIN_MOTOR_W {0.2};

constexpr uint8_t MAX_PWM {160};
constexpr float MAX_MOTOR_V {MAX_PWM/1.6};

constexpr float W2PWM {255/10};

constexpr uint8_t IDEAL_LIDAR_DIST {100};
constexpr uint8_t MIN_SIDE_DIST {20};

constexpr uint8_t MIN_FRONT_DIST {10};
constexpr uint8_t FRONT_DIST_SLOW_DOWN {30};

constexpr uint8_t IMU_ACTIVE_DIST {35};
