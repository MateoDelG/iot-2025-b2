#include "tempManager.h"
#include <OneWire.h>
#include <DallasTemperature.h>


namespace TempSensor {
  // Define el pin donde está conectado el bus OneWire
  #define ONE_WIRE_BUS 23

  // Configura el bus OneWire para la comunicación con el sensor
  OneWire oneWire(ONE_WIRE_BUS);

  // Instancia un objeto para manejar sensores de temperatura Dallas
  DallasTemperature tempSensors(&oneWire);

  // Función para inicializar el sensor de temperatura
  void begin() {
    tempSensors.begin();  // Inicializa la comunicación con el sensor
  }

  // Función para leer la temperatura en grados Celsius
  float readTemperatureC() {
    tempSensors.requestTemperatures(); // Envía una solicitud para leer la temperatura
    float temperatura_C = tempSensors.getTempCByIndex(0); // Obtiene la temperatura del primer sensor conectado

    // Imprime la temperatura en el monitor serial
    Serial.print("Temperatura: ");
    Serial.print(temperatura_C);
    Serial.println(" °C");

    return temperatura_C; // Devuelve la temperatura en grados Celsius
  }
}