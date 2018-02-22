#ifndef Car_4WD_H
#define Car_4WD_H

#include "Car.h"
#include "L293D_Motor.h"
#include <Servo.h>

#define SERVO_PWM_PIN 10

class Car_4WD : public Car
{
  private:
    byte wheel_pin;
    Servo wheel;
    L293D_Motor lf;
    L293D_Motor lb;
    L293D_Motor rf;
    L293D_Motor rb;

  public:
    Car_4WD(byte lfNum = 1, byte lbNum = 2, byte rbNum = 3, byte rfNum = 4, byte wheelPin = SERVO_PWM_PIN);
    void attachWheel();
    void run(State dir);
    void turn(byte angle);
    void uturn(State dir);
    void stop();
    void changeSpeed(byte speed);
};

#endif


