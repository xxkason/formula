#ifndef Motor_H
#define Motor_H

#include "Arduino.h"

#define MAX_SPEED 255
#define MIN_SPEED 0

enum State
{
  FOR = 1,
  BACK = 2,
  STOP = 3
};

class Motor
{
  protected:
    State motor_state;
  public:
    virtual void stop() = 0;
    virtual void run(State dir) = 0;
    virtual int setSpeed(int speed) = 0;
};

#endif
