#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "PubSubClient.h"
#include "sensorsManager.h"
#include "global_control.h"

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
      delay(500);        // Espera medio segundo antes de volver a comprobar
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
    client.setCallback(callback);             // Establecemos la función de callback para manejar mensajes entrantes
  }

  // Función para reconectar al servidor MQTT y suscribirse al tópico deseado
  void reconnect()
  {
    bool subscription = false;
    const String topic = "microsumo/control"; // Topic al que nos suscribiremos

    // Loop hasta que estemos reconectados y suscritos
    while (!client.connected() || !subscription)
    {
      Serial.println("Esperando a la conexión MQTT...");
      // Intentamos conectar con el servidor MQTT
      // String cliebnt_id = "ESP32Client-" + String(random(0xffff), HEX);
      // Obtenemos un ID único para el cliente basado en la dirección MAC
      if (client.connect(RobotControl.getId().c_str()))
      {
        // Si la conexión es exitosa, intentamos suscribirnos al topic
        if (client.subscribe(topic.c_str()))
        {
          subscription = true;
          Serial.println("Suscrito" + topic); // Confirmación de suscripción
        }
      }
      else
      {
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
    static unsigned long current_time = 0;    // Variable para almacenar el tiempo desde el último mensaje
    const int publish_interval = 5000;        // Intervalo de publicación en milisegundos (5 segundos)
    String topic_publish = "microsumo/state"; // Topic donde se publicará el mensaje

    // Publica un mensaje cada 5 segundos
    if (millis() > current_time + publish_interval)
    {

      // Publica el valor del sensor en el topic MQTT
      client.publish(topic_publish.c_str(), RobotControl.getId().c_str());

      Serial.println("mensaje enviado: " + RobotControl.getId()); // Imprime el mensaje enviado al monitor serial
      current_time = millis();                                    // Actualiza el tiempo del último mensaje enviado
    }
  }

  String buildJSONMessage()
  {
    JsonDocument doc;
    doc["id"] =  RobotControl.getId(); // Añade el ID del robot al documento JSON

    //movimiento
    JsonObject movement = doc.createNestedObject("movement"); // Crea un objeto anidado para el movimiento
    movement["direction"] = RobotControl.getDirection(); // Añade la dirección del movimiento al objeto "movement"
    


    String output;
    serializeJson(doc, output);
    return output; // Serializa el documento JSON a una cadena y la devuelve
  }

  void publishJSON(){

    const String  topic_publish = "microsumo/state"; // Topic donde se publicará el mensaje JSON
    static unsigned long current_time = 0;    // Variable para almacenar el tiempo desde el último mensaje
    const int publish_interval = 5000;        // Intervalo de publicación en milisegundos (5 segundos)

    // Publica un mensaje cada 5 segundos
    if (millis() > current_time + publish_interval)
    {
      // Publica el valor del sensor en el topic MQTT
      client.publish(topic_publish.c_str(), buildJSONMessage().c_str()); // Publica el mensaje JSON en el topic MQTT
      Serial.println("mensaje enviado: " + buildJSONMessage()); // Imprime el mensaje enviado al monitor serial
      current_time = millis();                                    // Actualiza el tiempo del último mensaje enviado
    }
  }

  void messageToJSON(const char *jsonMessage)
  {
    JsonDocument doc; // Creamos un documento JSON para almacenar los datos
    DeserializationError error = deserializeJson(doc, jsonMessage); // Deserializamos el mensaje JSON

    if (error)
    {
      Serial.print(F("Error al deserializar JSON: "));
      Serial.println(error.c_str()); // Imprimimos el error si la deserialización falla
      return; // Salimos de la función si hay un error
    }

    if(doc["id"].isNull())
    {
      Serial.println("ID no encontrado en el mensaje JSON"); // Imprimimos un mensaje si el ID no está presente
      return; // Salimos de la función si el ID no está presente
    }
    else
    {
      Serial.println("ID encontrado: " + String(doc["id"].as<const char*>())); // Imprimimos el ID encontrado en el mensaje JSON
    }

    JsonObject json = doc.as<JsonObject>(); // Convertimos el documento JSON a un objeto JSON
    if(json.containsKey("movement"))
    {
      JsonObject movement = doc["movement"]; // Obtenemos el objeto "movement" del mensaje JSON
      if(movement.containsKey("direction"))
      {
        String direction = movement["direction"].as<String>(); // Obtenemos la dirección del movimiento
        RobotControl.setDirection(direction); // Actualizamos la dirección del robot
        Serial.println("Dirección actualizada: " + direction); // Imprimimos la dirección actualizada
      }
      else
      {
        Serial.println("Dirección no encontrada en el mensaje JSON"); // Imprimimos un mensaje si la dirección no está presente
      }
    }

  }

  // Función callback que se ejecuta cuando llega un mensaje MQTT al ESP32
  void callback(char *topic, byte *payload, unsigned int length)
  {
    // Imprimir el topic del mensaje recibido
    Serial.print("Mensaje recibido [");
    Serial.print(topic);
    Serial.print("]: ");

    // Crear un buffer para el mensaje y convertir el payload en una cadena
    char message[length + 1]; // +1 para el carácter nulo '\0'
    for (unsigned int i = 0; i < length; i++)
    {
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
    const String control_topic = "microsumo/control"; // Tópico de control
    if (topicStr == control_topic)
    {
      messageToJSON(message); // Procesar el mensaje JSON si el tópico es de control
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
    publishJSON();     // Llama a la función de publicación de datos del sensor
    client.loop(); // Mantiene la conexión activa y procesa mensajes entrantes
  }
}