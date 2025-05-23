#include <Arduino.h>
#include <WiFi.h>
#include "PubSubClient.h"
#include "sensorsManager.h"
#include "MotorsController.h"
namespace WifiManager
{
  // Definimos el nombre y la contraseña de la red WiFi a la que se conectará el ESP32
  const char *ssid = "Delga";         // Nombre de la red WiFi (SSID)
  const char *password = "Delga1213"; // Contraseña de la red WiFi

  // Dirección del servidor MQTT al que nos conectaremos
  const char *mqtt_server = "broker.mqtt.cool";
  // Puerto del servidor MQTT (1883 es el puerto estándar no seguro)
  #define MQTT_PORT 1883

  // Creamos un cliente WiFi y un cliente MQTT usando la librería PubSubClient
  WiFiClient espClient;
  PubSubClient client(espClient);

  // Declaración anticipada de la función callback (se define más abajo)
  void callback(char *topic, byte *payload, unsigned int length);

  // Función para conectar el ESP32 a la red WiFi
  void setupWifi()
  {
    // Iniciamos la conexión WiFi con el SSID y contraseña definidos
    WiFi.begin(ssid, password);
    // Esperamos hasta que el ESP32 se conecte a la red WiFi
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500); // Espera medio segundo antes de volver a comprobar
      Serial.print("."); // Imprime un punto para indicar que sigue intentando
    }
    Serial.println("");
    Serial.println("WiFi connected."); // Mensaje de éxito
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP()); // Mostramos la IP asignada al ESP32
  }

  // Función para configurar el cliente MQTT
  void setupMqtt()
  {
    client.setServer(mqtt_server, MQTT_PORT); // Configuramos el servidor MQTT y el puerto
    client.setCallback(callback); // Establecemos la función de callback para manejar mensajes entrantes
  }

  // Función para reconectar al servidor MQTT y suscribirse al tópico deseado
  void reconnect() {
    bool subscription = false;
    const String topic = "microsumo/function_mode"; // Topic al que nos suscribiremos

    // Loop hasta que estemos reconectados y suscritos
    while (!client.connected() || !subscription) {
      Serial.println("Esperando a la conexión MQTT...");
      // Intentamos conectar con el servidor MQTT
      if (client.connect("ESP32Client")) {
        // Si la conexión es exitosa, intentamos suscribirnos al topic
        if (client.subscribe(topic.c_str())) {
          subscription = true;
          Serial.println("Suscrito" + topic); // Confirmación de suscripción
        }
      } else {
        // Si falla la conexión, mostramos el código de error y esperamos 5 segundos
        Serial.print("fallo, rc=");
        Serial.print(client.state());
        Serial.println(" intentar de nuevo en 5 segundos");
        delay(5000); // Espera 5 segundos antes de intentar reconectar
      }
    }
  }

  // Función para publicar datos del sensor en un tópico MQTT
  void publish()
  {
    static unsigned long current_time = 0; // Variable para almacenar el tiempo desde el último mensaje
    const int publish_interval = 5000; // Intervalo de publicación en milisegundos (5 segundos)
    String topic_publish = "microsumo/sensores/line_sensor"; // Topic donde se publicará el mensaje

    // Publica un mensaje cada 5 segundos
    if (millis() > current_time + publish_interval)
    {
      // Lee el valor del sensor de línea
      int sensor_value = SensorsManager::readRawLineSensor();

      char msg[16]; // Buffer para almacenar la representación del número como cadena

      // Convierte el número entero a cadena y almacénalo en msg
      snprintf(msg, sizeof(msg), "%d", sensor_value);
      // Publica el valor del sensor en el topic MQTT
      client.publish(topic_publish.c_str(), msg);

      Serial.println("mensaje enviado: " + String(msg)); // Imprime el mensaje enviado al monitor serial
      current_time = millis();     // Actualiza el tiempo del último mensaje enviado
    }
  }

  // Función callback que se ejecuta cuando llega un mensaje MQTT al ESP32
  void callback(char *topic, byte *payload, unsigned int length) {
    // Imprimir el topic del mensaje recibido
    Serial.print("Mensaje recibido [");
    Serial.print(topic);
    Serial.print("]: ");

    // Crear un buffer para el mensaje y convertir el payload en una cadena
    char message[length + 1]; // +1 para el carácter nulo '\0'
    for (unsigned int i = 0; i < length; i++) {
      message[i] = (char)payload[i];
    }
    message[length] = '\0'; // Terminar la cadena

    // Imprimir el valor como texto
    Serial.println(message);

    // Convertimos el topic y el mensaje a String para facilitar el manejo
    String topicStr = String(topic);
    String valueStr = String(message);

    // Imprimimos los valores recibidos
    Serial.println("==> Topic recibido: " + topicStr);
    Serial.println("==> Valor recibido: " + valueStr);

    // Tomar decisiones según el tópico y el valor recibido
    const String control_topic = "microsumo/function_mode";
    if (topicStr == control_topic) {
      int modo = valueStr.toInt(); // Convertir a entero si es un número
      Serial.print("Modo de funcionamiento: ");
      Serial.println(modo);

      // Según el valor recibido, ejecutamos una acción en los motores
      if (modo == 99) {
        MotorsController::automaticMode();
        MotorsController::stop();
      } else if (modo == 8) {
        MotorsController::forward();
        delay(1000); // Avanza 1 segundo
        MotorsController::stop();
      } else if (modo == 2) {
        MotorsController::backward();
        delay(1000); // Retrocede 1 segundo
        MotorsController::stop();
      } else if (modo == 4) {
        MotorsController::left();
        delay(300); // Gira a la izquierda 0.3 segundos
        MotorsController::stop();
      } else if (modo == 6) {
        MotorsController::right();
        delay(300); // Gira a la derecha 0.3 segundos
        MotorsController::stop();
      } else if (modo == 0) {
        MotorsController::stop();
      } else {
        Serial.println("Modo no reconocido");
      }
    }
  }

  // Función principal del servicio MQTT, debe llamarse en el loop principal
  void MQTTService()
  {
    // Verifica la conexión al servidor MQTT y reconecta si es necesario
    if (!client.connected())
    {
      reconnect();
    }
    publish();     // Llama a la función de publicación de datos del sensor
    client.loop(); // Mantiene la conexión activa y procesa mensajes entrantes
  }
}