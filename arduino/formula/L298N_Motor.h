#ifndef L298N_Motor_H
#define L298N_Motor_H

#include "Motor.h"

class L298N_Motor : public Motor
{
    private:
    int in1, in2, pwm, speed;

    public:
    L298N_Motor(int in1Pin, int in2Pin, int pwmPin);
    ~L298N_Motor();
    void stop() = 0;
    void run(Direction dir) = 0;
    int setSpeed(int speed) = 0;
}

#endif