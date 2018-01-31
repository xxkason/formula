#include "L2HBd_Motor.h"

L2HBd_Motor::L2HBd_Motor(int in1Pin, int in2Pin)
{
    in1 = in1Pin;
    in2 = in2Pin;
    speed = MAX_SPEED;
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
}

L2HBd_Motor::~L2HBd_Motor(){}

void L2HBd_Motor::stop()
{
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
}

void L2HBd_Motor::run(Direction dir)
{
    if (dir == FOR)
    {
        analogWrite(in1, speed);
        digitalWrite(in2, LOW);
    }
    else if (dir == BACK)
    {
        digitalWrite(in1, LOW);
        analogWrite(in2, speed);
    }
}

int L2HBd_Motor::setSpeed(int value)
{
    if (value > MAX_SPEED)
        speed = MAX_SPEED;
    else if (value < MIN_SPEED)
        speed = MIN_SPEED;
    else
        speed = value;
    return speed;
}