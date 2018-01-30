#include "Car_2DC.h"

Car_2DC::Car_2DC(int wheelNum, int driverNum): wheel(wheelNum), driver(driverNum)
{
  wheel.setSpeed(WHEEL_MAX);
  driver.setSpeed(MAX_SPEED);
  wheel.run(RELEASE);
  driver.run(RELEASE);
}

Car_2DC::~Car_2DC() {}

void Car_2DC::stop()
{
  wheel.run(RELEASE);
  driver.run(RELEASE);
  Serial.println("stoped");
}

void Car_2DC::run(directions dir)
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

void Car_2DC::turn(directions dir)
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

int Car_2DC::changeSpeed(int value)
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

