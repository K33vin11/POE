#include <Wheel.h>
#include <Config.h>


Wheel::Wheel(uint8_t i2cAddress, int encAPin, int encBPin, float dia, bool invert)
    : motor(i2cAddress, invert), encoder(encAPin, encBPin, invert), diameter(dia) {}


void Wheel::begin() {
    encoder.begin();
    motor.begin();
}

void Wheel::setSpeed(int speed) {
    motor.setSpeed(speed);
}

void Wheel::stop(String direction) {
    motor.stop(direction);
}
void Wheel::stop() {
    motor.stop("fw");
}

float Wheel::getDistanceMoved() {
    long pulses = encoder.readPulses();
    float distancePerPulse = (PI_PI * diameter * 100.0) / ENCODER_CPR; // Durchmesser in m → *100 = cm
    return pulses * distancePerPulse;
}

void Wheel::reset() {
    encoder.reset();
}


