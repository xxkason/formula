#ifndef Car_H
#define Car_H

#include "Arduino.h"

#define MAX_SPEED 255
#define MIN_SPEED 0

enum directions
{
  QIAN,
  HOU,
  ZUO,
  YOU
};

class Car
{
  public:
    virtual void run(directions dir) = 0;
    virtual void turn(directions dir) = 0;
    virtual void stop() = 0;
    virtual int changeSpeed(int speed) = 0;
};

#endif
