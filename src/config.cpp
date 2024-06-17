/********************************************************************************/
/*                                                                              */
/*                            RUTINA CONFIG                                     */
/*                                                                              */
/********************************************************************************/

#include "Relgeta.h"

extern regleta_config_t config;



// CARGA DE PARÁMETROS
void config_load_parameters()
{
    config.tiempo_menu = DEFAULT_TIEMPO_MENU; //Tiempo que se muetra el menu
    config.tiempo_ini = DEFAULT_TIEMPO_INI; //Momento de inicio
    config.ID_regleta = DEFAULT_ID_CAJA_str; //Identificador de la regleta
    config.ssid = DEFAULT_ssid; //SSID wifi
    config.passwifi = DEFAULT_password; //PASS wifi
    strlcpy(config.broker, DEFAULT_broker, sizeof(config.broker)); //IP BROKER
    config.port_mqtt = DEFAULT_port_mqtt; //Puerto acceso broker
    strlcpy(config.mqttUsername, DEFAULT_mqttUsername, sizeof(config.mqttUsername)); //Usuario acceso broker
    strlcpy(config.mqttPassword, DEFAULT_mqttPassword, sizeof(config.mqttPassword)); //Pass acceso Broker
}
