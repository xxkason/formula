#ifndef L293D_Motor_H
#define L293D_Motor_H

#include "Motor.h"
#include <AFMotor.h>

class L293D_Motor : public Motor
{
    private:
    int speed;
    AF_DCMotor motor;

    public:
    L293D_Motor(int motorNum);
    ~L293D_Motor();
    void stop() = 0;
    void run(Direction dir) = 0;
    int setSpeed(int speed) = 0;
}

#endif