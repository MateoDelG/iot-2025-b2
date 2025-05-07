#include <Arduino.h>
#include "SensorSimulado.h"

// >varName:1234\n Formato esperado por Teleplot

void imprimirMensaje1();
void imprimirMensaje2();

void setup() {
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  SensorSimulado::iniciarSensorTemperatura();
}



void loop() {
  // Serial.print(">Temp:");
  // Serial.println(SensorSimulado::leerTemperatura());
  // delay(500);

  imprimirMensaje1();
  imprimirMensaje2();
}


void imprimirMensaje1(){
 static unsigned long tiempo_actual = millis(); // Obtener el tiempo actual en milisegundos
 const int tiempo_actualizacion = 2000; // Tiempo de actualización en milisegundos

  if (millis() - tiempo_actual >= tiempo_actualizacion) { // Verificar si ha pasado el tiempo de actualización
      Serial.println("Mensaje 1: Actualizando datos...");
      tiempo_actual = millis(); // Actualizar el tiempo actual
  }
}

void imprimirMensaje2(){
  static unsigned long tiempo_actual = millis(); // Obtener el tiempo actual en milisegundos
  const int tiempo_actualizacion = 500; // Tiempo de actualización en milisegundos
 
   if (millis() - tiempo_actual >= tiempo_actualizacion) { // Verificar si ha pasado el tiempo de actualización
       Serial.println("      Mensaje 2: Actualizando datos...");
       tiempo_actual = millis(); // Actualizar el tiempo actual
   }
 }
