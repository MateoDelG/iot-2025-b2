#include <Arduino.h>
#include "WifiManager.h"
#include "MotorsController.h"
#include "sensorsManager.h"



void setup() {
  Serial.begin(115200); // Inicia la comunicación serial a 115200 baudios
  delay(7000);
  Serial.println("Start"); // Indica el inicio del programa en el monitor serial
  MotorsController::setup(); // Llama a la función de configuración de motores
  SensorsManager::setupLine(); // Configura el sensor de líne
  WifiManager::setupWifi(); // Llama a la función de configuración de WiFi
  WifiManager::setupMqtt(); // Configura el servidor MQTT y el puerto
}

void loop() {
  WifiManager::MQTTService(); // Llama a la función de servicio MQTT
}
