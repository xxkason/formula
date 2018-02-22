#ifndef CarFactory_H
#define CarFactory_H

#include "Car_2DC.h"
#include "Car_4WD.h"

class CarFactory
{
    public:
        static Car createCar(String car);
}

#endif