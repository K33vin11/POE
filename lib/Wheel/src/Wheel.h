#ifndef WHEEL_H
#define WHEEL_H

#include <Motor.h>
#include <Encoder.h>

class Wheel {
private:
    Motor motor;
    Encoder encoder;
    float diameter;
    bool invert;
public:
Wheel(uint8_t i2cAddress, int encAPin, int encBPin, float dia, bool invert = false);
    void begin();
    void setSpeed(int speed);
    void stop(String direction);
    float getDistanceMoved();
    void reset(); // Diese Methode brauchst du für Encoder-Reset
    void stop(); // Stoppt den Motor ohne Richtung

};

#endif
