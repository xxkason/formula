#ifndef Car_H
#define Car_H

#include "Motor.h"

class Car
{
  public:
    virtual void run(Direction dir) = 0;
    virtual void turn(Direction dir) = 0;
    virtual void stop() = 0;
    virtual int changeSpeed(int speed) = 0;
};

#endif
