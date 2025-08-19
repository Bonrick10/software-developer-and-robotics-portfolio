#pragma once

#include "Motion.hpp"
#include "config/config-stringCommand.hpp"
#include <ctype.h>

namespace mtrn3100 {

class StringCommand {
public:
  StringCommand(Motion &m, const char* initStrCmd = "")
    : motion(m), state(STATE_COND), currX(0), currY(0), currH(m.getCurrIMUDeg()), i(0) {
    setStrCmd(initStrCmd);
  }

  void run() {
    switch (state) {
      case STATE_COND:
        if (i >= MAX_STR_CMD_LENGTH) state = STATE_END;
        currX = motion.getCurrOdomX();
        currY = motion.getCurrOdomY();
        currCmd = toupper(strCmd[i]);
        if (currCmd == 'F') {
          if (prevState != state) currH = motion.getCurrIMUDeg();
          state = STATE_F;
        } else if (currCmd == 'R') {
          currH = motion.getCurrIMUDeg();
          state = STATE_R;
        } else if (currCmd == 'L') {
          currH = motion.getCurrIMUDeg();
          state = STATE_L;
        }
        prevState = state;
        break;
      case STATE_F:
        if (motion.moveDistance(250, currX, currY, currH)) {
          motion.setMotion(0,0);
          delay(500);
          i++;
          state = STATE_COND;
        };
        break;
      case STATE_R:
        if (motion.moveAngle(-90, currH)) {
          motion.setMotion(0,0);
          delay(500);
          i++;
          state = STATE_COND;
        }
        break;
      case STATE_L:
        if (motion.moveAngle(90, currH)) {
          motion.setMotion(0,0);
          delay(500);
          i++;
          state = STATE_COND;
        }
        break;
      case STATE_END:
        motion.setMotion(0,0);
        break;
      default:
        state = STATE_COND;
        break;
    }
  }

  void setStrCmd(const char* newStrCmd) {
    strncpy(strCmd, newStrCmd, sizeof(strCmd) - 1);
    strCmd[sizeof(strCmd) - 1] = '\0';
  }

private:
  Motion &motion;
  enum CommandState {STATE_COND=1, STATE_F, STATE_R, STATE_L, STATE_END};

  CommandState state;
  CommandState prevState;
  char strCmd[MAX_STR_CMD_LENGTH + 1];
  char currCmd;
  float currX, currY, currH;
  int i;
};

} // namespace mtrn3100
