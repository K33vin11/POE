#ifndef DRIVESYSTEM_H
#define DRIVESYSTEM_H

#include <Wheel.h>
#include <EdgeSensor.h>
#include <Gyro.h>
#include <Arduino.h>

struct Location {
    float x;      
    float y;      
    float theta;  
};

class DriveSystem {
private:
    Wheel* wheels[4];
    EdgeSensor* edgeSensors[6];
    Gyro* gyro;
    float posX;
    float posY;
    float orientation; 
    float speed;
    float targetHeading = NAN;
    float startwinkel;
    float abweichung;
    float angleDifference(float target, float current) {
    float diff = fmod((current - target + 540.0), 360.0) - 180.0;
    return diff;
    String dirB;
}

public:
    DriveSystem(Wheel* w1, Wheel* w2, Wheel* w3, Wheel* w4,
                EdgeSensor* sensors[5],
                Gyro* gyro);
    void begin();
    void move(float vx, float vy, float omega);
    void stop(String direction);
    bool moveToLeftEdge();
    bool moveToRightEdge();
    bool moveToRightEdge(float endOdy);
    bool moveToFrontEdge();
    bool moveToBackEdge();
    float getCurrentOrientation();
    void updatePosition();
    void resetOdometry();
    Location getCurrentLocation();
    bool goTo(float targetX, float targetY); 
    bool turn(float targetAngle); 
    void setSpeedLimit(float limit);
    bool moveBack(float mills);
    bool moveForward(float mills);
    void moveCorrect(float vx, float vy);
    void stop();
    bool searchBridge(String dirB);
    bool moveAlongRightEdge();  
    bool moveAlongRightEdgeBack();  
    bool moveBack(float mills,float startAngle);
    bool moveAlongRightEdge(int time);
    bool moveAlongRightEdgeBack(int time);



};

#endif
