#include "Car_4WD.h"

Car_4WD::Car_4WD(byte lfNum, byte lbNum, byte rbNum, byte rfNum, byte wheelPin):lf(lfNum),lb(lbNum),rb(rbNum),rf(rfNum)
{
  //Servo pin attach must do in setup() function in the sketch
  //if you do the attach in contructor, 
  //it causes them to rotate all the way to one side
  //refer to https://stackoverflow.com/questions/17335592/arduino-putting-servos-in-my-class-causes-them-to-rotate-all-the-way-to-one-sid
  //wheel.attach(wheelPin);
  //wheel.write(CENTER_POSITION);
  wheel_pin = wheelPin;
  angle = CENTER_POSITION;
}

void Car_4WD::attachWheel() //call this method in setup() function
{
  wheel.attach(wheel_pin);
  wheel.write(CENTER_POSITION);
}

void Car_4WD::stop()
{
  //wheel.write(CENTER_POSITION);
  lf.stop();
  lb.stop();
  rb.stop();
  rf.stop();
}

void Car_4WD::run(State dir)
{
  lf.run(dir);
  lb.run(dir);
  rb.run(dir);
  rf.run(dir);
}

void Car_4WD::turn(byte value)
{
  if (value > RIGHT_POSITION)
    angle = RIGHT_POSITION;
  else if (value < LEFT_POSITION)
    angle = LEFT_POSITION;
  else
    angle = value;
  wheel.write(angle);
}

void Car_4WD::uturn(State dir)
{
  if (dir == FOR)
  {
    wheel.write(RIGHT_POSITION);
    lf.run(FOR);
    lb.run(FOR);
    rf.stop();
    rb.stop();
  }
  else if (dir = BACK)
  {
    wheel.write(LEFT_POSITION);
    rf.run(FOR);
    rb.run(FOR);
    lf.stop();
    lb.stop();
  }
}

void Car_4WD::changeSpeed(byte value)
{
  lf.setSpeed(value);
  lb.setSpeed(value);
  rb.setSpeed(value);
  rf.setSpeed(value);
}
