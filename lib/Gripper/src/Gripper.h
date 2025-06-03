#ifndef GRIPPER_H
#define GRIPPER_H


class Gripper {
private:
    int motorPin;
    int dirPin; 
    int dir;
    int openP;
    int closeP;
    int servoPin; 

public:
    Gripper(int motorPin, int directionPin, bool direction,int openPin, int closePin);
    void begin();
    void open(int time);
    void close(int time);
    void Down(int time); 
    void Up(int time); 
};

#endif
