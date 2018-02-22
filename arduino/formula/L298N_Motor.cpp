#include "L298N_Motor.h"

L298N_Motor::L298N_Motor(byte in1Pin, byte in2Pin, byte pwmPin)
{
    in1 = in1Pin;
    in2 = in2Pin;
    pwm = pwmPin;
    speed = MAX_SPEED;
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(pwm, OUTPUT);
    stop();
    motor_state = STOP;
}

void L298N_Motor::stop()
{
    if (motor_state != STOP)
    {
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        analogWrite(pwm, MIN_SPEED);
        motor_state = STOP;
    }
}

void L298N_Motor::run(State dir)
{
    if (motor_state == dir)
        return;
    
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
    motor_state = dir;
}

byte L298N_Motor::setSpeed(byte value)
{
    if (value > MAX_SPEED)
        speed = MAX_SPEED;
    else if (value < MIN_SPEED)
        speed = MIN_SPEED;
    else
        speed = value;
    return speed;
}
