#include "Car_4WD.h"

Car_4WD::Car_4WD(): lfDrive(1), l
{
  wheel.attach(SERVO_PWM_PIN);
 lfMotor.setSpeed(MAX_SPEED);
 lbMotor.setSpeed(MAX_SPEED);
 rfMotor.setSpeed(MAX_SPEED);
 rbMotor.setSpeed(MAX_SPEED);
 wheel.write(wheelAngle);
 lfMotor.run(RELEASE);
 lbMotor.run(RELEASE);
 rfMotor.run(RELEASE);
 rbMotor.run(RELEASE);
}

Car_4WD::~Car_4WD(){}

void Car_4WD::stop()
{
  wheel.run(RELEASE);
  driver.run(RELEASE);
  Serial.println("stoped");
}

void Car_4WD::run(directions dir)
{
  if (dir == QIAN)
  {
    driver.run(FORWARD);
    Serial.println("qian jin");
  }
  else if (dir == HOU)
  {
    driver.run(BACKWARD);
    Serial.println("hou tui");
  }
}

void Car_4WD::turn(directions dir)
{
  if (dir == ZUO)
  {
    wheel.run(FORWARD);
    Serial.println("zuo guai");
  }
  else if (dir == YOU)
  {
    wheel.run(BACKWARD);
    Serial.println("you guai");
  }
}

int Car_4WD::changeSpeed(int value)
{
  if (value > 255)
    speed = 255;
  else if (value < 0)
    speed = 0;
  else
    speed = value;
  driver.setSpeed(speed);
  return speed;
}
