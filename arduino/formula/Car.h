#ifndef Car_H
#define Car_H

#include "Arduino.h"
#include <AFMotor.h>

enum run_direction
{
  forward,
  backward
};

enum turn_direction
{
  left,
  right
};

class Car_2DC
{
  public:
    Car_2DC(int num1, int num2);
    ~Car_2DC();
    void run();
    void turn();
    void stop();
    void changeSpeed(int speed);
  private:
    AF_DCMotor wheel(1);
    AF_DCMotor driver(2);
};

#endif
