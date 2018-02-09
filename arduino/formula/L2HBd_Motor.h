#ifndef L2HBd_Motor_H
#define L2HBd_Motor_H

#include "Motor.h"

class L2HBd_Motor : public Motor
{
  private:
    int in1, in2, speed;

  public:
    L2HBd_Motor(int in1Pin, int in2Pin);
    ~L2HBd_Motor();
    void stop();
    void run(State dir);
    int setSpeed(int speed);
};

#endif
