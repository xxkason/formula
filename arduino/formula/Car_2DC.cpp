#include "Car_2DC.h"

Car_2DC::Car_2DC(int wheelNum, int driverNum)
{
  L293D_Motor wheel293D(wheelNum);
  L293D_Motor drive293D(driverNum);
  wheel293D.setSpeed(WHEEL_MAX);
  wheel = & wheel293D;
  drive = & drive293D;
}

Car_2DC::Car_2DC(int wheelIn1Pin, int wheelIn2Pin, int wheelPWMPin, int driveIn1Pin, int driveIn2Pin, int drivePWMPin)
{
  L298N_Motor wheel298N(wheelIn1Pin, wheelIn2Pin, wheelPWMPin);
  L298N_Motor drive298N(driveIn1Pin, driveIn2Pin, drivePWMPin);
  wheel298N.setSpeed(WHEEL_MAX);
  wheel = & wheel298N;
  drive = & drive298N;
}

Car_2DC::Car_2DC(int wheelIn1Pin, int wheelIn2Pin, int driveIn1Pin, int driveIn2Pin)
{
  L2HBd_Motor wheel2hbd(wheelIn1Pin, wheelIn2Pin);
  L2HBd_Motor drive2hbd(driveIn1Pin, driveIn2Pin);
  wheel2hbd.setSpeed(WHEEL_MAX);
  wheel = & wheel2hbd;
  drive = & drive2hbd;
}

Car_2DC::~Car_2DC() {}

void Car_2DC::stop()
{
  wheel->stop();
  drive->stop();
  Serial.println("stoped");
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

