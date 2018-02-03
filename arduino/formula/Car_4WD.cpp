#include "Car_4WD.h"

Car_4WD::Car_4WD(int lfNum, int lbNum, int rbNum, int rfNum, int wheelPin):lf(lfNum),lb(lbNum),rb(rbNum),rf(rfNum)
{
  //Servo pin attach must do in setup() function in the sketch
  //if you do the attach in contructor, 
  //it causes them to rotate all the way to one side
  //refer to https://stackoverflow.com/questions/17335592/arduino-putting-servos-in-my-class-causes-them-to-rotate-all-the-way-to-one-sid
  //wheel.attach(wheelPin);
  //wheel.write(CENTER_POSITION);
  wheel_pin = wheelPin;
}

Car_4WD::~Car_4WD() {}

void Car_4WD::attachWheel() //call this method in setup() function
{
  wheel.attach(wheel_pin);
  wheel.write(CENTER_POSITION);
}

void Car_4WD::stop()
{
  wheel.write(CENTER_POSITION);
  lf.stop();
  lb.stop();
  rb.stop();
  rf.stop();
}

void Car_4WD::run(Direction dir)
{
  lf.run(dir);
  lb.run(dir);
  rb.run(dir);
  rf.run(dir);
}

void Car_4WD::turn(Direction dir)
{
  if (dir == FOR)
  {
    wheel.write(RIGHT_POSITION);
    //Serial.println("wheel servo turn left");
  }
  else if (dir == BACK)
  {
    wheel.write(LEFT_POSITION);
    //Serial.println("wheel servo turn right");
  }
}

void Car_4WD::uturn(Direction dir)
{
  if (dir == FOR)
  {
    lf.run(FORWARD);
    lb.run(FORWARD);
    rf.run(BACKWARD);
    rb.run(BACKWARD);
    wheel.write(RIGHT_POSITION);
  }
  else if (dir = BACK)
  {
    rf.run(FORWARD);
    rb.run(FORWARD);
    lf.run(BACKWARD);
    lb.run(BACKWARD);
    wheel.write(LEFT_POSITION);
  }
}

void Car_4WD::shift(Direction dir){}

int Car_4WD::changeSpeed(int value)
{
  lf.setSpeed(value);
  lb.setSpeed(value);
  rb.setSpeed(value);
  return rf.setSpeed(value);
}
