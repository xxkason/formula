#ifndef L2HBd_Motor_H
#define L2HBd_Motor_H

#include "Motor.h"

class L2HBd_Motor : public Motor
{
  private:
    byte in1, in2, speed;

  public:
    L2HBd_Motor(byte in1Pin, byte in2Pin);
    void stop();
    void run(State dir);
    byte setSpeed(byte speed);
};

#endif
