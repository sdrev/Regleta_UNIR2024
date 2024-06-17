/********************************************************************************/
/*                                                                              */
/*                           SETTINGS DEL MQTT                                  */
/*                                                                              */
/********************************************************************************/

//#include <Arduino.h>
#include "Relgeta.h"
#include "WiFi.h"
#include "PubSubClient.h"

extern regleta_config_t config;

extern boolean DEBUG;

String topic_ESTADO_regleta;
String topic_KEEPALIVE;
String topic_GLOBAL;
String topic_PETICION;

uint8_t ultimoIntentoRecon = 0;

WiFiClient cliente;

PubSubClient mqtt(cliente);


// SETEO DE LA CONEXION MQTT
void mqttSettings()
{
  topic_GLOBAL = "#"; // #
  topic_ESTADO_regleta =  config.ID_regleta + "/ESTADO"; // REGLETAXX/ESTADO
  topic_KEEPALIVE =  config.ID_regleta + "/KEEPALIVE"; // REGLETAXX/KEEPALIVE
  topic_PETICION = config.ID_regleta; // REGLETAXX

  mqtt.setServer(config.broker, config.port_mqtt);
  mqtt.setCallback(mqttCallback);
  mqttConnect();
  keepalive();
}

// Comprobación de la Conexion
boolean mqttConnect()
{

  printInfoln(config.ID_regleta,DEBUG);
  printInfo("Conectando a ",DEBUG);
  printInfoln(config.broker,DEBUG);

  // CONEXION CON username Y password
  boolean status = mqtt.connect(config.ID_regleta.c_str(), config.mqttUsername, config.mqttPassword);  
  printInfoln((String)status,DEBUG);

  if (status == false)
  {
    printInfoln("La conexión no ha sido posible", DEBUG);
    return false;
  }
  else
  {
    mqtt.subscribe(topic_GLOBAL.c_str(), 0);
    printInfoln(topic_GLOBAL.c_str(),DEBUG);
    printInfoln(topic_KEEPALIVE.c_str(),DEBUG);
    printInfoln(topic_ESTADO_regleta.c_str(),DEBUG);
    printInfoln(topic_PETICION.c_str(),DEBUG);   
  }

  return mqtt.connected();
}

// keepalive para comunicar al dashboard
void keepalive()
{
  mqtt.publish(topic_KEEPALIVE.c_str(), "OK");
  delay(100);
}

// Lectura mensaje publicado
void mqttCallback(char *topic, byte *message, unsigned int len)
{
  printInfoln("",DEBUG);
  printInfoln("",DEBUG);
  printInfo("MENSAJE PUBLICADO EN EL TOPIC: ",DEBUG);
  printInfoln(topic,DEBUG);
  printInfo("MENSAJE:    ",DEBUG);
  String messageTemp;

  for (int i = 0; i < len; i++)
  {
    printInfo((String)((char)message[i]),DEBUG);
    messageTemp += (char)message[i];
  }
  printInfoln("",DEBUG);

  if (String(topic) == topic_PETICION + "/SALIDA1")
  {
    if (messageTemp == "1")
    {
      // actualizar_estado
      printInfoln("ACTIVANDO SALIDA1.",DEBUG);
      digitalWrite(RELAY1, 1);
      delay(100);
    }
    else if (messageTemp == "0")
    {
      // actualizar_estado
      printInfoln("DESACTIVANDO SALIDA1.",DEBUG);
      digitalWrite(RELAY1, 0);
      delay(100);
    }
  }
  else if (String(topic) == topic_PETICION + "/SALIDA2")
  {
    if (messageTemp == "1")
    {
      // actualizar_estado
      printInfoln("ACTIVANDO SALIDA2.",DEBUG);
      digitalWrite(RELAY2, 1);
      delay(100);
    }
    else if (messageTemp == "0")
    {
      // actualizar_estado
      printInfoln("DESACTIVANDO SALIDA2.",DEBUG);
      digitalWrite(RELAY2, 0);
      delay(100);
    }
  }
  else if (String(topic) == topic_PETICION + "/SALIDA3")
  {
    if (messageTemp == "1")
    {
      // actualizar_estado
      printInfoln("ACTIVANDO SALIDA3.",DEBUG);
      digitalWrite(RELAY3, 1);
      delay(100);
    }
    else if (messageTemp == "0")
    {
      // actualizar_estado
      printInfoln("DESACTIVANDO SALIDA3.",DEBUG);
      digitalWrite(RELAY3, 0);
      delay(100);
    }
  }
  else if (String(topic) == topic_PETICION + "/ESTADO"){
    if (messageTemp == "SALIDAS"){
      mqttEstadoActual();
    }
  }

}

// Devolver estado de las salidas
void mqttEstadoActual(){
  mqtt.publish((topic_ESTADO_regleta + "/SALIDA1").c_str() , ((String)(digitalRead(RELAY1))).c_str());
  mqtt.publish((topic_ESTADO_regleta + "/SALIDA2").c_str() , ((String)(digitalRead(RELAY2))).c_str());
  mqtt.publish((topic_ESTADO_regleta + "/SALIDA3").c_str() , ((String)(digitalRead(RELAY3))).c_str());
}

// Mantener conexió
void mqttMantenerConexion()
{
  if (!mqtt.loop())
  {
    mqttConnect();
    ultimoIntentoRecon++;
  }
  else
  {
    ultimoIntentoRecon = 0;
  }
}
