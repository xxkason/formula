#include "Car_4WD.h"

Car_4WD::Car_4WD(int lfNum, int lbNum, int rfNum, int rbNum, int wheelPin):lf(lfNum),lb(lbNum),rf(rfNum),rb(rbNum)
{
  wheel.attach(wheelPin);
  wheel.write(CENTER_POSITION);
  lfDrive = & lf;
  lbDrive = & lb;
  rfDrive = & rf;
  rbDrive = & rb;
}

Car_4WD::~Car_4WD() {}

void Car_4WD::stop()
{
  wheel.write(CENTER_POSITION);
  lfDrive->stop();
  lbDrive->stop();
  rbDrive->stop();
  rfDrive->stop();
}

void Car_4WD::run(Direction dir)
{
  lfDrive->run(dir);
  lbDrive->run(dir);
  rbDrive->run(dir);
  rfDrive->run(dir);
}

void Car_4WD::turn(Direction dir)
{
  if (dir == FOR)
  {
    wheel.write(RIGHT_POSITION);
    Serial.println("wheel servo turn left");
  }
  else if (dir == BACK)
  {
    wheel.write(LEFT_POSITION);
    Serial.println("wheel servo turn right");
  }
}

void Car_4WD::uturn(Direction dir){}

void Car_4WD::shift(Direction dir){}

int Car_4WD::changeSpeed(int value)
{
  lfDrive->setSpeed(value);
  lbDrive->setSpeed(value);
  rbDrive->setSpeed(value);
  return rfDrive->setSpeed(value);
}
