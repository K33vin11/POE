#include "Robot.h"
#include "config.h"

Robot::Robot(DriveSystem* drive, Gripper* grip, BridgePlacer* bridge, DistanceSensor* sensor)
    : driveSystem(drive), gripper(grip), bridgePlacer(bridge), distanceSensor(sensor) {}

void Robot::begin() {
    Serial.println("Roboter initialisieren...");
    if (driveSystem) driveSystem->begin();
    delay(1000);
    if (gripper) gripper->begin();
    delay(1000);
    if (bridgePlacer) bridgePlacer->begin();
    delay(1000);
    if (distanceSensor) distanceSensor->begin();
    delay(1000);
    Serial.println("Roboter bereit!");
}

void Robot::move(float vx, float vy, float omega) {
    if (driveSystem) driveSystem->move(vx, vy, omega);
}

void Robot::stop(const String& direction) {
    if (driveSystem) {
        if (direction.length() == 0)
            driveSystem->stop();
        else
            driveSystem->stop(direction);
    }
}



void Robot::bridgeUp() {
    if (bridgePlacer) bridgePlacer->Up(BRIDGE_UP_TIME);
}

void Robot::bridgeUp(int extraTime) {
    if (bridgePlacer) bridgePlacer->Up(BRIDGE_UP_TIME + extraTime);
}

void Robot::bridgeDown() {
    if (bridgePlacer) bridgePlacer->Down(BRIDGE_DOWN_TIME);
}
void Robot::gripperDown(){
    if (gripper) gripper->Down(GRIPPER_MOTOR_DOWN_TIME);
}

void Robot::gripperUp() {
    if (gripper) gripper->Up(GRIPPER_MOTOR_UP_TIME);
}
void Robot::gripperClose() {
    if (gripper) gripper->close(GRIPPER_CLOSE_TIME);
}
void Robot::gripperOpen() {
    if (gripper) gripper->open(GRIPPER_OPEN_TIME);
}



bool Robot::turn(float targetAngle) {
    if (driveSystem) return driveSystem->turn(targetAngle);
    return false;
}

void Robot::setSpeedLimit(float limit) {
    if (driveSystem) driveSystem->setSpeedLimit(limit);
}

bool Robot::moveToRightEdge(float startOdy) {
    if (driveSystem) return driveSystem->moveToRightEdge(startOdy);
    return false; 
}

bool Robot::moveToLeftEdge() {
    if (driveSystem) return driveSystem->moveToLeftEdge();
    return false;
}

bool Robot::moveToFrontEdge() {
    if (driveSystem) return driveSystem->moveToFrontEdge();
    return false;
}

bool Robot::moveToBackEdge() {
    if (driveSystem) return driveSystem->moveToBackEdge();
    return false;
}

bool Robot::moveBack(float mills) {
    if (driveSystem) return driveSystem->moveBack(mills);
    return false;
}

bool Robot::moveForward(float mills) {
    if (driveSystem) return driveSystem->moveForward(mills);
    return false;
}

bool Robot::moveAlongRightEdge() {
    if (driveSystem) return driveSystem->moveAlongRightEdge();
    return false;
}

bool Robot::moveAlongRightEdge(int time) {
    if (driveSystem) return driveSystem->moveAlongRightEdge(time);
    return false;
}

bool Robot::moveAlongRightEdgeBack(int time) {
    if (driveSystem) return driveSystem->moveAlongRightEdgeBack(time);
    return false;
}

bool Robot::moveAlongRightEdgeBack() {
    if (driveSystem) return driveSystem->moveAlongRightEdgeBack();
    return false;
}


bool Robot::moveBack(int time, float startOdy) {
    if (driveSystem) return driveSystem->moveBack(time, startOdy);
    return false;
}

bool Robot::objectInGripper() {
    return (digitalRead(GRIPPER_OBJECT_PIN) == LOW) || (distanceSensor && distanceSensor->getDistance() < 10 && distanceSensor->getDistance() > 0);
}