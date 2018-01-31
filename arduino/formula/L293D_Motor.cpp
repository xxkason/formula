#include "L293D_Motor.h"

L293D_Motor::L293D_Motor(int motorNum): motor(motorNum)
{
  motor.setSpeed(MAX_SPEED);
  motor.run(RELEASE);
}

L293D_Motor::~L293D_Motor() {}

void L293D_Motor::stop()
{
  motor.run(RELEASE);
}

void L293D_Motor::run(Direction dir)
{
  motor.run(dir);
}

int L293D_Motor::setSpeed(int value)
{
  if (value > MAX_SPEED)
    speed = MAX_SPEED;
  else if (value < MIN_SPEED)
    speed = MIN_SPEED;
  else
    speed = value;
  motor.setSpeed(speed);
  return speed;
}
