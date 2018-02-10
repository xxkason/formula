#ifndef Car_2DC_H
#define Car_2DC_H

#include "Car.h"
#include "L293D_Motor.h"
#include "L298N_Motor.h"
#include "L2HBd_Motor.h"

#define WHEEL_MAX 160

class Car_2DC : public Car
{
  protected:
    Motor *wheel;
    Motor *drive;

  public:
    void run(State dir);
    void turn(State dir);
    void stop();
    void changeSpeed(int speed);
};

class Car_2DC_L293D : public Car_2DC
{
  private:
    L293D_Motor wheel293D;
    L293D_Motor drive293D;
  public:
    Car_2DC_L293D(int wheelNum, int driverNum);
};

class Car_2DC_L298N : public Car_2DC
{
  private:
    L298N_Motor wheel298N;
    L298N_Motor drive298N;
  public:
    Car_2DC_L298N(int wheelIn1Pin, int wheelIn2Pin, int wheelPWMPin, int driveIn1Pin, int driveIn2Pin, int drivePWMPin);
};

class Car_2DC_L2HBd : public Car_2DC
{
  private:
    L2HBd_Motor wheel2hbd;
    L2HBd_Motor drive2hbd;
  public:
    Car_2DC_L2HBd(int wheelIn1Pin, int wheelIn2Pin, int driveIn1Pin, int driveIn2Pin);
};

#endif


