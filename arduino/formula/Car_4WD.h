#ifndef Car_4WD_H
#define Car_4WD_H

#include "Car.h"
#include "L293D_Motor.h"
#include <Servo.h>

#define SERVO_PWM_PIN 10
#define RIGHT_POSITION 180
#define LEFT_POSITION 0
#define CENTER_POSITION 55

class Car_4WD : public Car
{
  private:
    int wheel_pin;
    int speed;
    Servo wheel;
    L293D_Motor lf;
    L293D_Motor lb;
    L293D_Motor rf;
    L293D_Motor rb;

  public:
    Car_4WD(int lfNum = 1, int lbNum = 2, int rbNum = 3, int rfNum = 4, int wheelPin = SERVO_PWM_PIN);
    ~Car_4WD();
    void attachWheel();
    void run(Direction dir);
    void turn(Direction dir);
    void uturn(Direction dir);
    void shift(Direction dir);
    void stop();
    int changeSpeed(int speed);
};

#endif


