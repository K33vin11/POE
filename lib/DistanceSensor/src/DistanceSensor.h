#ifndef DISTANCESENSOR_H
#define DISTANCESENSOR_H

#include <NewPing.h>

class DistanceSensor {
private:
    int trigPin;
    int echoPin;
    NewPing* sonar;
    float detectionThreshold; 
public:
    DistanceSensor(int trig, int echo, float threshold);
    void begin();
    float getDistance();
    bool objectDetected();
};

#endif
