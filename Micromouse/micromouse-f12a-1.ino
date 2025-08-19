/**
 * MTRN3100 24T2
 * F12A-1: Jack Arman, Chak Wang Choi, Kyle Hill, Beonrik Pascual
 */

#include <Wire.h>
#include "src/config/config-setup.hpp"
#include "src/MicroMouse.hpp"

/* -------------------------------------------------------------------------- */
/*                              SETUP MICROMOUSE                              */
/* -------------------------------------------------------------------------- */

mtrn3100::MicroMouse myMouse(
  Wire,
  SENSOR1_PIN, SENSOR1_ADDRESS,
  SENSOR2_PIN, SENSOR2_ADDRESS,
  SENSOR3_PIN, SENSOR3_ADDRESS,
  SCREEN_ADDRESS,
  MOT1PWM, MOT1DIR,
  MOT2PWM, MOT2DIR,
  MOT1ENCA, MOT1ENCB,
  MOT2ENCA, MOT2ENCB,
  WHEEL_RADIUS, BASE_LENGTH
);

void setup() {
  Serial.begin(115200);
  Wire.begin();
  myMouse.setup();
}

/* -------------------------------------------------------------------------- */
/*                               LOOP OPERATIONS                              */
/* -------------------------------------------------------------------------- */

void loop() {
  myMouse.run();
}
