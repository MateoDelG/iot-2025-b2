#include "SensorSimulado.h"
#include <Arduino.h>


namespace SensorSimulado{
    int temperaturaAPorcentaje(float temperatura);

    void iniciarSensorTemperatura(){
        randomSeed(analogRead(19)); // Inicializa la semilla aleatoria con el valor de un pin analógico no conectado
        Serial.println("Sensor de temperatura iniciado.");
    }

    float leerTemperatura(){
        // Simula la lectura de un sensor de temperatura
        float temperatura = random(5, 50) + random(0, 100) / 100.0; // Genera un valor entre 20.00 y 30.99
        return temperaturaAPorcentaje(temperatura); // Convierte la temperatura a porcentaje (0-100%)
    }


    int temperaturaAPorcentaje(float temperatura){
        int porcentaje = map(temperatura, 5, 50, 0, 100);
        return porcentaje;
    }

}