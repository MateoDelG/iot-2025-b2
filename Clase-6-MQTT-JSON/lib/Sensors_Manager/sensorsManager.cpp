#include "sensorsManager.h"

#define LINE_SENSOR_PIN 0

namespace SensorsManager {
void setupLine() { pinMode(LINE_SENSOR_PIN, INPUT); }

bool readLineSensor() {
  const int lineThreshold = 3800;
  int lineSensorValue = analogRead(LINE_SENSOR_PIN);
  Serial.println("Line sensor value: " + String(lineSensorValue));

  if (lineSensorValue >= lineThreshold) {
    return true; // Line detected
  } else {
    return false;
  }
}

// Función para leer la temperatura interna del ESP32 (aproximada)
int readRawLineSensor() {
  int lineSensorValue = analogRead(LINE_SENSOR_PIN);
  // Serial.println("Line sensor value: " + String(lineSensorValue));
  return lineSensorValue;
}

} // namespace SensorsManager