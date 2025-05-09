#ifndef TEMP_MANAGER_H
#define TEMP_MANAGER_H

#include <Arduino.h>

namespace TempSensor {
  void begin();
  float readTemperatureC();
}

#endif