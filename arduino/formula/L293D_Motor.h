#ifndef L293D_Motor_H
#define L293D_Motor_H

#include "Motor.h"
#include <AFMotor.h>

class L293D_Motor : public Motor
{
  private:
    byte speed;
    AF_DCMotor motor;

  public:
    L293D_Motor(byte motorNum);
    void stop();
    void run(State dir);
    byte setSpeed(byte speed);
};

#endif
