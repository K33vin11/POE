#ifndef BRIDGEPLACER_H
#define BRIDGEPLACER_H


class BridgePlacer {
private:
    int servoPin;
    int dirPin; 
    bool dir; 
public:
    BridgePlacer(int motorPin, int directionPin, bool direction);
    void begin();
    void Down(int time);
    void Up(int time);
};

#endif
