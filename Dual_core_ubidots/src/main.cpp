#include <Arduino.h>
#include <WiFi.h>
#include "WiFiManager.h"
#include "ubidotsESP32Mqtt.h"



const char *ssid = "Delga"; // Reemplaza con tu SSID
const char *password = "Delga1213"; // Reemplaza con tu contraseña

const char *Ubidots_Token = "BBUS-k2DesIYqrGRk5133NrNl748KpgD6Nv";
#define DEVICE_LABEL "iot_b2_2025"
#define VARIABLE_LABEL "temp-test"

TaskHandle_t handleTarea1;
TaskHandle_t handleTarea2;

Ubidots ubidots(Ubidots_Token);

WiFiManager wifiManager;

void loopNucleo0(void *parameter);
void loopNucleo1(void *parameter);
void taskDelay(int delay);
void resetWifiConfig();

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando...");

  // Conectar a la red Wi-Fi
//   WiFi.begin(ssid, password);
//   Serial.print("Conectando a la red Wi-Fi");
//   unsigned int long startTime = millis();
//   const int timeout = 10000; // 10 segundos
//   while (WiFi.status() != WL_CONNECTED && (millis() - startTime) < timeout) {
//     delay(1000);
//     Serial.print(".");
//   }
// if(WiFi.status() != WL_CONNECTED) {
//     Serial.println("No se pudo conectar a la red Wi-Fi");
//   }
//   else {
//     Serial.println("Conectado a la red Wi-Fi");
//   }
  // resetWifiConfig();

  // Conectar a la red Wi-Fi usando WiFiManager
  bool connected = wifiManager.autoConnect("ESP32_Ubidots"); // Nombre del punto de acceso

  if (connected) {
    Serial.println("Conectado a la red Wi-Fi");
  } else {
    Serial.println("No se pudo conectar a la red Wi-Fi");
  }

  // Crear tareas en el núcleo 1
  xTaskCreatePinnedToCore(
    loopNucleo0,   // Función de la tarea
    "Tarea Nucleo 0", // Nombre de la tarea
    10000,          // Tamaño de la pila (en bytes)
    NULL,           // Parámetro de entrada
    1,              // Prioridad de la tarea
    &handleTarea1,  // Handle de la tarea
    0);             // Núcleo al que se asigna (0 o 1)

  // Crear tareas en el núcleo 2
  xTaskCreatePinnedToCore(
    loopNucleo1,   // Función de la tarea
    "Tarea Nucleo 1", // Nombre de la tarea
    10000,          // Tamaño de la pila (en bytes)
    NULL,           // Parámetro de entrada
    1,              // Prioridad de la tarea
    &handleTarea2,  // Handle de la tarea
    1);             // Núcleo al que se asigna (0 o 1)


    ubidots.setDebug(true);
    ubidots.setup();
}

void loop() {}

void loopNucleo0(void *parameter) {
  while (true) {
    if (!ubidots.connected()) {
      Serial.println("Conectando a Ubidots...");
      ubidots.connect();
    }

    Serial.println("Hola desde el núcleo 0");
    float temperature = random(0,100); // Simulación de temperatura
    ubidots.add(VARIABLE_LABEL, temperature);
    ubidots.publish(DEVICE_LABEL);
    Serial.print("Enviando temperatura: ");

    taskDelay(60000); // Espera 1 segundo
  }
}

void loopNucleo1(void *parameter) {
  while (true) {
    Serial.println("Hola desde el núcleo 1");
    taskDelay(1000); // Espera 1 segundo
  }
}

void resetWifiConfig(){
  Serial.println("Reiniciando configuración de Wi-Fi...");
  wifiManager.resetSettings();
  Serial.println("Configuración de Wi-Fi reiniciada.");
}

void taskDelay(int delay){
  vTaskDelay(delay / portTICK_PERIOD_MS);
}