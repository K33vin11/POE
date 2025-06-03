#ifndef ROBOT_H
#define ROBOT_H

#include <Arduino.h>
#include <DriveSystem.h>
#include <Gripper.h>
#include <BridgePlacer.h>
#include <DistanceSensor.h>

class Robot {
private:
    DriveSystem* driveSystem;
    Gripper* gripper;
    BridgePlacer* bridgePlacer;
    DistanceSensor* distanceSensor;

public:
    Robot(DriveSystem* drive, Gripper* grip, BridgePlacer* bridge, DistanceSensor* sensor);

    void begin();

    void move(float vx, float vy, float omega);
    void stop(const String& direction = "");

    void gripperDown();
    void gripperUp();
    void gripperClose();
    void gripperOpen();


    void bridgeDown();
    void bridgeUp();
    void bridgeUp(int extraTime);


    bool goTo(float targetX, float targetY);
    bool turn(float targetAngle);

    void setSpeedLimit(float limit);

    bool moveToRightEdge(float startOdy);
    bool moveToLeftEdge();
    bool moveToFrontEdge();
    bool moveToBackEdge();

    bool moveBack(float mills);
    bool moveForward(float mills);
    bool moveBack(int time, float startOdy);

    bool moveAlongRightEdge();
    bool moveAlongRightEdge(int time);
    bool moveAlongRightEdgeBack();
    bool moveAlongRightEdgeBack(int time);
    bool objectInGripper();


};

#endif
