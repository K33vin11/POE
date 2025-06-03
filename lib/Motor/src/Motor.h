#ifndef MOTOR_H
#define MOTOR_H

#include <Wire.h>

class Motor {
private:
    uint8_t i2cAddress;
    bool invert; 
public:
    Motor(uint8_t address, bool invertDirection = false);
    void begin();
    void setSpeed(int speed); 
    void stop(String direction = "fw");
    void sendSpeed(uint8_t direction, int speed);
    void stop();

};

#endif
