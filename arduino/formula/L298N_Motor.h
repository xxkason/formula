#ifndef L298N_Motor_H
#define L298N_Motor_H

#include "Motor.h"

class L298N_Motor : public Motor
{
  private:
    byte in1, in2, pwm, speed;

  public:
    L298N_Motor(byte in1Pin, byte in2Pin, byte pwmPin);
    void stop();
    void run(State dir);
    byte setSpeed(byte speed);
};

#endif
