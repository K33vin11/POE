#ifndef ENCODER_H
#define ENCODER_H

class Encoder {
private:
    int pinA, pinB;
    volatile long pulses;
    int encoderIndex;
    bool invert;
public:
    Encoder(int a, int b, bool invert);
    void begin();
    void handleInterrupt();
    long readPulses();
    void reset();
    float calculateDistance();
};

#endif
