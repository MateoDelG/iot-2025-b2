#ifndef GLOBAL_CONTROL_H
#define GLOBAL_CONTROL_H
#include <Arduino.h>

class GlobalControl
{

public:
    GlobalControl();

    String getId() const;

    //Movimiento
    String getDirection() const; // Get the direction of movement
    void setDirection(const String dir); // Set the direction of movement
    

private:
    String id; // Unique identifier for the control instance
    String direction; // Direction of movement (e.g., "left", "right", "forward", "backward")
};

extern GlobalControl RobotControl; // Declare a global instance of GlobalControl

#endif