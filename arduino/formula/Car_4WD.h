#ifndef Car_4WD_H
#define Car_4WD_H

#include "Car.h"
#include "L293D_Motor.h"
#include <Servo.h>

#define SERVO_PWM_PIN 10
#define RIGHT_POSITION 180
#define LEFT_POSITION 0
#define CENTER_POSITION 90

class Car_4WD : public Car
{
  private:
    int wheel_pin;
    int angle;
    Servo wheel;
    L293D_Motor lf;
    L293D_Motor lb;
    L293D_Motor rf;
    L293D_Motor rb;

  public:
    Car_4WD(int lfNum = 1, int lbNum = 2, int rbNum = 3, int rfNum = 4, int wheelPin = SERVO_PWM_PIN);
    ~Car_4WD();
    void attachWheel();
    void run(State dir);
    void turn(State dir);
    void uturn(State dir);
    void changeAngle(int angle);
    void stop();
    void changeSpeed(int speed);
    int currentAngle();
};

#endif


