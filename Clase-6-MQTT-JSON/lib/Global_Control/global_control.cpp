#include "global_control.h"


GlobalControl RobotControl;

GlobalControl::GlobalControl()
{
    // Initialize the unique identifier for the control instance
    id = "ESP32-" + String(random(0xffff), HEX);
    direction = "none"; // Default direction of movement
}

String GlobalControl::getId() const
{
    // Return the unique identifier for the control instance
    return id;
}

String GlobalControl::getDirection() const
{
    // Return the current direction of movement
    return direction;
}
void GlobalControl::setDirection(const String dir)
{
    direction = dir; // Set the direction of movement
}


//  {
//   "id": "ESP-002",
//   "movement": {
//     "direction": "left",
//     "motorActive": true
//   },
//   "sensor": {
//     "value": 780,
//     "threshold": 600
//   },
//   "system": {
//     "operationMode": "automatic",
//     "errorMessage": "None",
//     "systemActive": false
//   }
// }