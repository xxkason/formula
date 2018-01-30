#ifndef Car_2DC_H
#define Car_2DC_H

#include "Car.h"
#include <AFMotor.h>

#define WHEEL_MAX 160

class Car_2DC : public Car
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


