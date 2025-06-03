#ifndef GYRO_H
#define GYRO_H

#include <Arduino_NineAxesMotion.h>

class Gyro {
private:
    NineAxesMotion mySensor;
    float currentOrientation; 
public:
    Gyro();
    void begin();
    float getOrientation();
};

#endif
