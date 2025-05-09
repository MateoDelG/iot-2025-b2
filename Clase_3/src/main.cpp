#include <Arduino.h>
#include "tempManager.h"
#include "fanManager.h"

void updateFanSpeedWithTemperature();

void setup() {
  Serial.begin(115200); // Inicializa la comunicación serie
  TempSensor::begin(); // Inicializa el sensor de temperatura
  FanManager::begin(); // Inicializa el ventilador
  FanManager::turnOn(); // Enciende el ventilador
}

void loop() {
  updateFanSpeedWithTemperature(); // Actualiza la velocidad del ventilador según la temperatura
  delay(50);

} 


void updateFanSpeedWithTemperature() {
  // Lee la temperatura actual en grados Celsius desde el sensor
  float temperature = TempSensor::readTemperatureC();

  // Define los límites de temperatura para el control del ventilador
  const float Tmin = 28.0;  // Temperatura mínima donde el ventilador debe estar apagado
  const float Tmax = 35.0;  // Temperatura máxima donde el ventilador debe estar al 100%

  int speed; // Variable para almacenar la velocidad del ventilador en porcentaje (0–100)

  // Determina la velocidad del ventilador según la temperatura
  if (temperature < Tmin) {
    // Si la temperatura es menor que Tmin, el ventilador debe estar apagado
    speed = 0;
  } else if (temperature >= Tmax) {
    // Si la temperatura es mayor o igual a Tmax, el ventilador debe estar al 100%
    speed = 100;
  } else {
    // Si la temperatura está entre Tmin y Tmax, calcula la velocidad proporcionalmente
    float k = 100.0 / (Tmax - Tmin); // Factor de escalamiento
    speed = (int)((temperature - Tmin) * k); // Cálculo proporcional
  }

  // Ajusta la velocidad del ventilador usando el valor calculado
  FanManager::setFanSpeed(speed);

  // Enciende o apaga el ventilador según la velocidad calculada
  if (speed > 0 && !FanManager::isFanOn()) {
    // Si la velocidad es mayor a 0 y el ventilador está apagado, enciéndelo
    FanManager::turnOn();
  } else if (speed == 0 && FanManager::isFanOn()) {
    // Si la velocidad es 0 y el ventilador está encendido, apágalo
    FanManager::turnOff();
  }

  // Imprime información de depuración en el teleplot 
  Serial.print(">Temp (C):"); // Muestra la temperatura actual
  Serial.println(temperature);
  Serial.print(">PWM %:"); // Muestra la velocidad del ventilador en porcentaje
  Serial.println(speed);
}