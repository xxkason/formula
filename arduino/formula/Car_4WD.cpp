#include "Car_4WD.h"

Car_4WD::Car_4WD(): lfDrive(1), lbDrive(2), rfDrive(3), rbDrive(4)
{
  wheel.attach(SERVO_PWM_PIN);
  lfDrive.setSpeed(MAX_SPEED);
  lbDrive.setSpeed(MAX_SPEED);
  rfDrive.setSpeed(MAX_SPEED);
  rbDrive.setSpeed(MAX_SPEED);
  wheel.write(CENTER_POSITION);
  lfDrive.run(RELEASE);
  lbDrive.run(RELEASE);
  rfDrive.run(RELEASE);
  rbDrive.run(RELEASE);
}

Car_4WD::~Car_4WD() {}

void Car_4WD::stop()
{
  wheel.write(CENTER_POSITION);
  lfDrive.run(RELEASE);
  lbDrive.run(RELEASE);
  rfDrive.run(RELEASE);
  rbDrive.run(RELEASE);
  Serial.println("stoped");
}

void Car_4WD::run(directions dir)
{
  if (dir == QIAN)
  {
    lfDrive.run(FORWARD);
    lbDrive.run(FORWARD);
    rfDrive.run(FORWARD);
    rbDrive.run(FORWARD);
    Serial.println("qian jin");
  }
  else if (dir == HOU)
  {
    lfDrive.run(BACKWARD);
    lbDrive.run(BACKWARD);
    rfDrive.run(BACKWARD);
    rbDrive.run(BACKWARD);
    Serial.println("hou tui");
  }
}

void Car_4WD::turn(directions dir)
{
  if (dir == ZUO)
  {
    wheel.write(MAX_POSITION);
    Serial.println("zuo guai");
  }
  else if (dir == YOU)
  {
    wheel.write(MIN_POSITION);
    Serial.println("you guai");
  }
}

void Car_4WD::uturn(directions dir){}

void Car_4WD::shift(directions dir){}

int Car_4WD::changeSpeed(int value)
{
  if (value > 255)
    speed = 255;
  else if (value < 0)
    speed = 0;
  else
    speed = value;
  lfDrive.setSpeed(speed);
  lbDrive.setSpeed(speed);
  rfDrive.setSpeed(speed);
  rbDrive.setSpeed(speed);
  return speed;
}
