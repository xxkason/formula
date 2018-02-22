#include "L293D_Motor.h"

L293D_Motor::L293D_Motor(byte motorNum): motor(motorNum)
{
  motor.setSpeed(MAX_SPEED);
  motor.run(RELEASE);
  motor_state = STOP;
}

void L293D_Motor::stop()
{
  if (motor_state != STOP)
  {
    motor.run(RELEASE);
    motor_state = STOP;
  }
}

void L293D_Motor::run(State dir)
{
  if (motor_state != dir)
  {
    motor.run(dir);
    motor_state = dir;
  }
}

byte L293D_Motor::setSpeed(byte value)
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
