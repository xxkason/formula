#ifndef Car_4WD_H
#define Car_4WD_H

#include "Arduino.h"
#include <AFMotor.h>
#include <Servo.h>

#define SERVO_PWM_PIN 9
#define MAX_POSITION 180
#define MIN_POSITION 0
#define CENTER_POSITION 90
#define MAX_SPEED 255
#define MIN_SPEED 0

enum directions
{
  QIAN,
  HOU,
  ZUO,
  YOU
};

class Car_4WD
{
  private:
    int speed;
    Servo wheel;
    AF_DCMotor lfDrive;
    AF_DCMotor lbDrive;
    AF_DCMotor rfDrive;
    AF_DCMotor rbDrive;

  public:
    Car_4WD();
    ~Car_4WD();
    void run(directions dir);
    void turn(directions dir);
    void stop();
    int changeSpeed(int speed);
};

#endif


