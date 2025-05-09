#ifndef FAN_MANAGER_H
#define FAN_MANAGER_H

namespace FanManager {
  void begin(); // Inicializa el ventilador
  void setFanSpeed(int speed); // Establece la velocidad del ventilador (0-255)
  void turnOn(); // Enciende el ventilador
  void turnOff(); // Apaga el ventilador
  int getFanSpeed(); // Obtiene la velocidad actual del ventilador
  bool isFanOn(); // Verifica si el ventilador está encendido o apagado
}

#endif