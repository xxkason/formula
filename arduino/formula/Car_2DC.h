#ifndef Car_2DC_H
#define Car_2DC_H

#include "Car.h"
#include "L293D_Motor.h"
#include "L298N_Motor.h"
#include "L2HBd_Motor.h"

#define WHEEL_MAX 160

class Car_2DC : public Car
{
  private:
    int speed;
    Motor *wheel;
    Motor *drive;

  public:
    Car_2DC(int wheelNum, int driverNum);
    Car_2DC(int wheelIn1Pin, int wheelIn2Pin, int wheelPWMPin, int driveIn1Pin, int driveIn2Pin, int drivePWMPin);
    Car_2DC(int wheelIn1Pin, int wheelIn2Pin, int driveIn1Pin, int driveIn2Pin);
    ~Car_2DC();
    void run(Direction dir);
    void turn(Direction dir);
    void stop();
    int changeSpeed(int speed);
};

#endif


