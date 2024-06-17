/********************************************************************************/
/*                                                                              */
/*                         ARCHIVO DE CABECERAS                                 */
/*                                                                              */
/********************************************************************************/

// Creación de la libreria
#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>


// DEFINICIONES
#define TINY_GSM_MODEM_SIM7000

#define SerialAT Serial1

//Pin de conexión de LOS RELES
#define RELAY1 32
#define RELAY2 19
#define RELAY3 18

#define UART_TX                     27
#define UART_RX                     26
#define SIMCARD_PWKEY               4

#define UART_BANUD_RATE             115200


//  OBJETO REGLETA
typedef struct {
    int tiempo_menu;
    int tiempo_ini;
    String ID_regleta;
    String ssid;
    String passwifi;
    char broker[30];          //mqtt
    int port_mqtt;            //mqtt
    char mqttUsername[30];    //mqtt
    char mqttPassword[30];    //mqtt
}regleta_config_t;



//  VALORES DEFAULT PARA OBJETO REGLETA
#define DEFAULT_TIEMPO_MENU 5000;
#define DEFAULT_TIEMPO_INI millis();

#define DEFAULT_ID_CAJA_str "REGLETA01"
#define DEFAULT_ssid "unir";
#define DEFAULT_password "secreto2024";
#define DEFAULT_broker "192.168.1.175"
#define DEFAULT_port_mqtt 2024
#define DEFAULT_mqttUsername "cvs"
#define DEFAULT_mqttPassword "unir2024"


//  FUNCIONES
void mqttCallback(char* topic, byte* message, unsigned int len);
boolean mqttConnect();
void mqttPolling();
void mqttSettings ();
void mqttMantenerConexion();
void mqttEstadoActual();
void keepalive();

void printInfo(String cadena, boolean debug);
void printInfoln(String cadena, boolean debug);

void delSMS(int slot);
void recibirSMS();
void processSMS(String smsString, char *callerIDbuffer);

void sim7000_INICIO(void);
void config_load_parameters();

#endif