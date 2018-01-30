#ifndef Motor_H
#define Motor_H

#include "Arduino.h"

#define MAX_SPEED 255
#define MIN_SPEED 0

enum Direction
{
    FOR = 1
    BACK = 2
}

class Motor
{
    public:
    virtual void stop() = 0;
    virtual void run(Direction dir) = 0;
    virtual int setSpeed(int speed) = 0;
}

#endif
