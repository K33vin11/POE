#ifndef EDGESENSOR_H
#define EDGESENSOR_H


class EdgeSensor {
private:
    int pin;
public:
    EdgeSensor(int p);
    void begin();
    bool detectEdge();
};

#endif
