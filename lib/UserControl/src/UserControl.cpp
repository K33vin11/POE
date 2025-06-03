#include <UserControl.h>
#include <Arduino.h>

UserControl::UserControl(int startPin)
    : pinStart(startPin) {}

void UserControl::begin() {
    pinMode(pinStart, INPUT_PULLUP);
}

bool UserControl::startPressed() {
    return digitalRead(pinStart) == LOW;
}