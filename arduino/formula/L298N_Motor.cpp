#include "L298N_Motor.h"

L298N_Motor::L298N_Motor(int in1Pin, int in2Pin, int pwmPin)
{
    in1 = in1Pin;
    in2 = in2Pin;
    pwm = pwmPin;
    speed = MAX_SPEED;
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(pwm, OUTPUT);
}

L298N_Motor::~L298N_Motor(){}

void L298N_Motor::stop()
{
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(pwm, MIN_SPEED);
}

void L298N_Motor::run(Direction dir)
{
    if (dir == FOR)
    {
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
    }
    else if (dir == BACK)
    {
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
    }
    analogWrite(pwm, speed);
}

int L298N_Motor::setSpeed(int value)
{
    if (value > MAX_SPEED)
        speed = MAX_SPEED;
    else if (value < MIN_SPEED)
        speed = MIN_SPEED;
    else
        speed = value;
    return speed;
}