#ifndef USERCONTROL_H
#define USERCONTROL_H

class UserControl {
private:
    int pinStart;

public:
    UserControl(int startPin);
    void begin();
    bool startPressed();

};

#endif
