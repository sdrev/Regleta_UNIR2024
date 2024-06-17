/********************************************************************************/
/*                                                                              */
/*                 SISTEMA DE CONTROL DE REGLETA                                */
/*                                                                              */
/********************************************************************************/

#include <Arduino.h>
#include "Relgeta.h" // Creado
#include "WiFi.h"d
#include <TinyGsmClient.h>

regleta_config_t config;


//VARIABLE PARA DEBUG
boolean DEBUG = true; //A true para printear por puerto serie


// VARIABLES PARA GSM
TinyGsm modem(SerialAT);
TinyGsmClient client(modem);

/********************************************************************************/
/*                                     RUTINA DE SETUP                          */
/********************************************************************************/
void setup() {
  Serial.begin(115200);
  delay(100);

  printInfoln("INICIANDO EL SISTEMA........", DEBUG);

  //*****************************************************************
  // PINES QUE CONTROLAN LAS SALIDAS
  //*****************************************************************
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  digitalWrite(RELAY1, 1);
  digitalWrite(RELAY2, 1);
  digitalWrite(RELAY3, 1);
  //*****************************************************************


  delay(1000);

  //*****************************************************************
  // CARGA DE PARÁMETROS DE CONFIGURACIÓN
  //*****************************************************************
  config_load_parameters();
  //*****************************************************************

  Serial.println("");
  Serial.println("Introduce la configuración deseada sino quiere cargar la de defecto:(Dejar para continuar solo 5seg)");
  Serial.println(" |-> tlfn: numerotlfn");
  Serial.println(" |-> ssid: nombressid");
  Serial.println(" |-> pass_wifi: passwifi");
  Serial.println(" |-> ipbroker: direccionipbroker");
  Serial.println(" |-> puerto: puertomqtt");
  Serial.println(" |-> usermqtt: nombre_usuario");
  Serial.println(" |-> passmqtt: contraseña_usuario");

  int tiempo = millis();
  while ((millis()-tiempo) < config.tiempo_menu)
  {
    if(Serial.available()>0){
      tiempo = 0;
      String contenido = Serial.readStringUntil('\n');
      if (contenido.indexOf("ssid:")>=0)
      {
        Serial.println("Nuevo ssid " + contenido.substring(contenido.indexOf(' ')+1));
        config.ssid = contenido.substring(contenido.indexOf(' ')+1);
      }
      else if (contenido.indexOf("passwifi:")>=0)
      {
        Serial.println("Nueva pass " );
        config.passwifi = contenido.substring(contenido.indexOf(' ')+1);
      }
      else if (contenido.indexOf("ipbroker:")>=0)
      {
        Serial.println("Nueva ip " + contenido.substring(contenido.indexOf(' ')+1));
        strlcpy(config.broker, (contenido.substring(contenido.indexOf(' ')+1)).c_str(), sizeof(config.broker));
      }
      else if (contenido.indexOf("puerto:")>=0)
      {
        Serial.println("Nuevo puerto " + contenido.substring(contenido.indexOf(' ')+1));
        config.port_mqtt = atoi((contenido.substring(contenido.indexOf(' ')+1)).c_str());
      }
      else if (contenido.indexOf("usermqtt:")>=0)
      {
        Serial.println("Nuevo usuario " + contenido.substring(contenido.indexOf(' ')+1));
        strlcpy(config.mqttUsername,(contenido.substring(contenido.indexOf(' ')+1)).c_str(), sizeof(config.mqttUsername));
      }
      else if (contenido.indexOf("passqmtt:")>=0)
      {
        Serial.println("Nueva pass ");
        strlcpy(config.mqttPassword,(contenido.substring(contenido.indexOf(' ')+1)).c_str(), sizeof(config.mqttPassword));
      }
      else{
        Serial.println("Configuracion no encontrada");
      }      
      tiempo = millis();
    }
  }

  printInfoln("",DEBUG);
  printInfo("[WiFi] Conectando a ",DEBUG);
  printInfoln(config.ssid,DEBUG);

  delay(500);
  WiFi.begin(config.ssid, config.passwifi);
  int Tiempo_intento = 2000;
  int Tiempo_inicio = millis();
  while (WiFi.status() != WL_CONNECTED ) {
    delay(500);
    printInfoln("[WiFi] Connectando al WiFi..",DEBUG);
    if((millis()-Tiempo_inicio > Tiempo_intento)){
      printInfoln("[WiFi] No se ha podido establcer conexion",DEBUG);
      printInfoln("Continuando ejecución",DEBUG);
      break;
    }
  }

  //*****************************************************************
  // INICIO DEL SEVICIO GSM
  //*****************************************************************
  sim7000_INICIO();
  delay(500);
  //*****************************************************************

  //*****************************************************************
  // INICIO DEL SEVICIO MQTT
  //*****************************************************************
  mqttSettings();
  //*****************************************************************
}

/********************************************************************************/
/*                                     RUTINA DE LOOP                           */
/********************************************************************************/
void loop() {
  if(millis()-config.tiempo_ini > 120000){
    config.tiempo_ini = DEFAULT_TIEMPO_INI;
    keepalive();
  }
  recibirSMS();
  mqttMantenerConexion();
  delay(1000);
}
