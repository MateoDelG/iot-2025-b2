#include "MotorsController.h"


#define MOTOR_R1 4
#define MOTOR_R2 3
#define MOTOR_L1 2
#define MOTOR_L2 1

// Definimos un espacio de nombres para organizar las funciones del controlador de motores
namespace MotorsController {

  // Configura los pines de los motores como salidas y detiene los motores al inicio
  void setup() {
    pinMode(MOTOR_R1, OUTPUT);
    pinMode(MOTOR_R2, OUTPUT);
    pinMode(MOTOR_L1, OUTPUT);
    pinMode(MOTOR_L2, OUTPUT);
    stop(); // Asegura que los motores estén detenidos al iniciar
  }

  // Hace que el robot avance hacia adelante
  void forward() {
    digitalWrite(MOTOR_R1, HIGH); // Motor derecho hacia adelante
    digitalWrite(MOTOR_R2, LOW);
    digitalWrite(MOTOR_L1, HIGH); // Motor izquierdo hacia adelante
    digitalWrite(MOTOR_L2, LOW);
  }

  // Hace que el robot retroceda
  void backward() {
    digitalWrite(MOTOR_R1, LOW);
    digitalWrite(MOTOR_R2, HIGH); // Motor derecho hacia atrás
    digitalWrite(MOTOR_L1, LOW);
    digitalWrite(MOTOR_L2, HIGH); // Motor izquierdo hacia atrás
  }

  // Detiene ambos motores
  void stop() {
    digitalWrite(MOTOR_R1, LOW);
    digitalWrite(MOTOR_R2, LOW);
    digitalWrite(MOTOR_L1, LOW);
    digitalWrite(MOTOR_L2, LOW);
  }

  // Gira el robot hacia la derecha
  void right() {
    digitalWrite(MOTOR_R1, HIGH);  // Motor derecho hacia adelante
    digitalWrite(MOTOR_R2, LOW);
    digitalWrite(MOTOR_L1, LOW);   // Motor izquierdo hacia atrás
    digitalWrite(MOTOR_L2, HIGH);
  }

  // Gira el robot hacia la izquierda
  void left() {
    digitalWrite(MOTOR_R1, LOW);   // Motor derecho hacia atrás
    digitalWrite(MOTOR_R2, HIGH);
    digitalWrite(MOTOR_L1, HIGH);  // Motor izquierdo hacia adelante
    digitalWrite(MOTOR_L2, LOW);
  }

  void automaticMode(){
    forward();
    delay(1000); // Avanza durante 1 segundo
    right();
    delay(1000); // Gira a la derecha durante 1 segundo
    forward();
    delay(1000); // Avanza durante 1 segundo
    right();
    delay(1000); // Gira a la derecha durante 1 segundo
    left();
    delay(1000); // Gira a la izquierda durante 1 segundo
    forward();
    delay(1000); // Avanza durante 1 segundo
  }


  
} // namespace MotorsController