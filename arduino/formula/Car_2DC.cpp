#include "Car_2DC.h"

const long TURNING_TIME = 50;

Car_2DC::Car_2DC()
{
  angle = CENTER_POSITION;
}

void Car_2DC::stop()
{
  wheel->stop();
  drive->stop();
}

void Car_2DC::run(State dir)
{
  drive->run(dir);
}

void Car_2DC::turn(byte dir)
{
  if (dir == 90)
    return;
  if (dir > 90)
    wheel->run(FOR);
  else if (dir < 90)
    wheel->run(BACK);
  delay(TURNING_TIME);
  wheel->stop();
}

void Car_2DC::changeSpeed(byte value)
{
  drive->setSpeed(value);
}

Car_2DC_L293D::Car_2DC_L293D(byte wheelNum, byte driverNum): wheel293D(wheelNum), drive293D(driverNum)
{
  wheel293D.setSpeed(WHEEL_MAX);
  wheel = & wheel293D;
  drive = & drive293D;
}

Car_2DC_L298N::Car_2DC_L298N(byte wheelIn1Pin, byte wheelIn2Pin, byte wheelPWMPin, byte driveIn1Pin, byte driveIn2Pin, byte drivePWMPin): wheel298N(wheelIn1Pin, wheelIn2Pin, wheelPWMPin), drive298N(driveIn1Pin, driveIn2Pin, drivePWMPin)
{
  wheel298N.setSpeed(WHEEL_MAX);
  wheel = & wheel298N;
  drive = & drive298N;
}

Car_2DC_L2HBd::Car_2DC_L2HBd(byte wheelIn1Pin, byte wheelIn2Pin, byte driveIn1Pin, byte driveIn2Pin): wheel2hbd(wheelIn1Pin, wheelIn2Pin), drive2hbd(driveIn1Pin, driveIn2Pin)
{
  wheel2hbd.setSpeed(WHEEL_MAX);
  wheel = & wheel2hbd;
  drive = & drive2hbd;
}
