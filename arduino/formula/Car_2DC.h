#ifndef Car_2DC_H
#define Car_2DC_H

#include "Arduino.h"
#include <AFMotor.h>

#define WHEEL_MAX 160
#define MAX_SPEED 255
#define MIN_SPEED 0

enum directions
{
  QIAN,
  HOU,
  ZUO,
  YOU
};

class Car_2DC
{
  private:
    int speed;
    AF_DCMotor wheel;
    AF_DCMotor driver;

  public:
    Car_2DC(int wheelNum, int driverNum);
    ~Car_2DC();
    void run(directions dir);
    void turn(directions dir);
    void stop();
    int changeSpeed(int speed);
};

#endif


