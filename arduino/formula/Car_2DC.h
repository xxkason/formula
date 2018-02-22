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
    Car_2DC();
    void run(State dir);
    void turn(byte dir);
    void stop();
    void changeSpeed(byte speed);
};

class Car_2DC_L293D : public Car_2DC
{
  private:
    L293D_Motor wheel293D;
    L293D_Motor drive293D;
  public:
    Car_2DC_L293D(byte wheelNum, byte driverNum);
};

class Car_2DC_L298N : public Car_2DC
{
  private:
    L298N_Motor wheel298N;
    L298N_Motor drive298N;
  public:
    Car_2DC_L298N(byte wheelIn1Pin, byte wheelIn2Pin, byte wheelPWMPin, byte driveIn1Pin, byte driveIn2Pin, byte drivePWMPin);
};

class Car_2DC_L2HBd : public Car_2DC
{
  private:
    L2HBd_Motor wheel2hbd;
    L2HBd_Motor drive2hbd;
  public:
    Car_2DC_L2HBd(byte wheelIn1Pin, byte wheelIn2Pin, byte driveIn1Pin, byte driveIn2Pin);
};

#endif


