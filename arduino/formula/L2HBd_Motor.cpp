#include "L2HBd_Motor.h"

L2HBd_Motor::L2HBd_Motor(byte in1Pin, byte in2Pin)
{
    in1 = in1Pin;
    in2 = in2Pin;
    speed = MAX_SPEED;
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    motor_state = STOP;
}

void L2HBd_Motor::stop()
{
    if (motor_state != STOP)
    {
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        motor_state = STOP;
    }   
}

void L2HBd_Motor::run(State dir)
{
    if (motor_state == dir)
        return;
    
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
    motor_state = dir;
}

byte L2HBd_Motor::setSpeed(byte value)
{
    if (value > MAX_SPEED)
        speed = MAX_SPEED;
    else if (value < MIN_SPEED)
        speed = MIN_SPEED;
    else
        speed = value;
    return speed;
}
