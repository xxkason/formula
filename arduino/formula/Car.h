#ifndef Car_H
#define Car_H

#include "Motor.h"

#define RIGHT_POSITION 180
#define LEFT_POSITION 0
#define CENTER_POSITION 90

class Car
{
  protected:
    byte speed;
    byte angle;
  public:
    virtual void run(State dir) = 0;
    virtual void turn(byte angle) = 0;
    virtual void stop() = 0;
    virtual void changeSpeed(byte speed) = 0;
    byte currentSpeed();
    byte currentAngle();
};

#endif
