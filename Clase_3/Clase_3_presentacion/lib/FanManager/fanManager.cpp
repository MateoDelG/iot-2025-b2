#include "fanManager.h"
#include <Arduino.h>

namespace FanManager {
// Pin GPIO del ESP32 donde está conectado el ventilador (por ejemplo, usando un
// transistor como interruptor)
const int FAN_PWM_PIN = 18;
// Canal de PWM utilizado (el ESP32 permite varios canales de PWM)
const int PWM_CHANNEL = 0;
// Frecuencia del PWM en Hz. 1000 Hz es adecuado para controlar ventiladores DC.
const int PWM_FREQ = 1000;
// Resolución del PWM en bits: 8 bits permiten valores de 0 a 255
const int PWM_RESOLUTION = 8;
// Variable para guardar la velocidad actual del ventilador (0 a 255)
int currentSpeed = 0;
// Estado del ventilador: encendido (true) o apagado (false)
bool fanState = false;

const int min_pwm = 255; // Valor mínimo de PWM (0%)
const int max_pwm = 0; // Valor máximo de PWM (100%)

// Función de inicialización del ventilador
void begin() {
  // Configura el canal PWM con la frecuencia y resolución definidas
  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  // Asocia el pin físico al canal PWM
  ledcAttachPin(FAN_PWM_PIN, PWM_CHANNEL);
  // Apaga el ventilador al iniciar
  turnOff();
}

// Función para establecer la velocidad del ventilador (0 a 100%)
void setFanSpeed(int speed) {
  const int min_speed = 0; // Velocidad mínima (0%)
  const int max_speed = 100; // Velocidad máxima (100%)
  // Limita el valor a un rango válido (0 a 100%)
  speed = constrain(speed, min_speed, max_speed);
  // Convierte el valor de 0-100% a 0-255 (para PWM de 8 bits)
  // Nota: aquí se invierte el valor (255 = apagado, 0 = máximo encendido)
  // por el efecto "not" del transistor
  speed = map(speed, min_speed, max_speed, min_pwm, max_pwm);
  // Guarda la velocidad como estado actual
  currentSpeed = speed;
  // Si el ventilador está encendido, actualiza la señal PWM
  if (fanState) {
    ledcWrite(PWM_CHANNEL, speed);
  }
}
// Enciende el ventilador con la velocidad previamente establecida
void turnOn() {
  fanState = true;
  ledcWrite(PWM_CHANNEL, currentSpeed);
}
// Apaga el ventilador (PWM al 100%, que en este caso equivale a apagado)
void turnOff() {
  fanState = false;
  ledcWrite(PWM_CHANNEL, min_pwm); // 255 = señal baja para la mayoría de
                               // ventiladores conectados por transistor
}
// Devuelve la velocidad actual del ventilador (valor PWM real entre 0 y 255)
int getFanSpeed() { return currentSpeed; }
// Devuelve si el ventilador está encendido o no
bool isFanOn() { return fanState; }

} // namespace FanManager

