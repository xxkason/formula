#ifndef Car_H
#define Car_H

#include "Motor.h"

class Car
{
  protected:
    int speed;
  public:
    virtual void run(State dir) = 0;
    virtual void turn(State dir) = 0;
    virtual void stop() = 0;
    virtual void changeSpeed(int speed) = 0;
    int currentSpeed();
};

#endif
