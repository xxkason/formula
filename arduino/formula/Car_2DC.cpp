#include "Car_2DC.h"

void Car_2DC::stop()
{
  wheel->stop();
  drive->stop();
}

void Car_2DC::run(Direction dir)
{
  drive->run(dir);
}

void Car_2DC::turn(Direction dir)
{
  wheel->run(dir);
}

int Car_2DC::changeSpeed(int value)
{
  return drive->setSpeed(value);
}

Car_2DC_L293D::Car_2DC_L293D(int wheelNum, int driverNum): wheel293D(wheelNum), drive293D(driverNum)
{
  wheel293D.setSpeed(WHEEL_MAX);
  wheel = & wheel293D;
  drive = & drive293D;
}

Car_2DC_L298N::Car_2DC_L298N(int wheelIn1Pin, int wheelIn2Pin, int wheelPWMPin, int driveIn1Pin, int driveIn2Pin, int drivePWMPin): wheel298N(wheelIn1Pin, wheelIn2Pin, wheelPWMPin), drive298N(driveIn1Pin, driveIn2Pin, drivePWMPin)
{
  wheel298N.setSpeed(WHEEL_MAX);
  wheel = & wheel298N;
  drive = & drive298N;
}

Car_2DC_L2HBd::Car_2DC_L2HBd(int wheelIn1Pin, int wheelIn2Pin, int driveIn1Pin, int driveIn2Pin): wheel2hbd(wheelIn1Pin, wheelIn2Pin), drive2hbd(driveIn1Pin, driveIn2Pin)
{
  wheel2hbd.setSpeed(WHEEL_MAX);
  wheel = & wheel2hbd;
  drive = & drive2hbd;
}
