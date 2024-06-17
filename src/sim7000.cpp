/********************************************************************************/
/*                                                                              */
/*                  RUTINA PARA inicializar sim 800/ sim 7600                   */
/*                              E.D.O. 2023                                     */
/*                              Version 2.0                                     */
/*                                                                              */
/********************************************************************************/

#include "Relgeta.h"
#include "TinyGSM.h"
#include "Adafruit_FONA.h"

// COMUNICACION HARDWARE SERIAL CON GPRS Y SIM7000
HardwareSerial *sim7000Serial = &Serial1;
Adafruit_FONA sim7000 = Adafruit_FONA(SIMCARD_PWKEY);

extern regleta_config_t config;
extern TinyGsm modem;

extern boolean DEBUG;

// Función de arranque modulo Sim7000
void sim7000_INICIO(void)
{
  printInfoln("INICIANDO RUTINA MODULO SIM.......",DEBUG);

  sim7000Serial->begin(UART_BANUD_RATE, SERIAL_8N1, UART_RX, UART_TX); // ESTABLECEMOS LA CONEXION SERIAL CON EL CHIP

  if (!sim7000.begin(*sim7000Serial))
  {
    printInfoln(F("MODULO SIM NO ENCONTRADO"),DEBUG);
    Serial.flush();
    ESP.restart();
  }
  printInfoln(F("MODULO SIM OK!"),DEBUG);  

  delay(500);

  printInfoln("Esperando a la red...",DEBUG);

  sim7000.print(F("AT+CNMP=2\r\n"));
  if (!modem.waitForNetwork())
  {
    delay(5000);
    return;
  }

  if (modem.isNetworkConnected())
  {
    printInfoln("Red conectada",DEBUG);
  }

  String modem_name = modem.getModemName();
  String modem_info = modem.getModemInfo();


  printInfoln("\r\n\r\n\r\n",DEBUG);
  printInfoln(F("***************************************************************************"),DEBUG);
  printInfo("***                        DATOS                                          ***\n",DEBUG);
  printInfoln(F("***************************************************************************"),DEBUG);
  printInfoln(F("***                                                                     ***"),DEBUG);
  printInfo(F("*** MODULO SIM:                         "),DEBUG); // Imprime el NOMBRE DEL MODULO
  printInfo(modem_name,DEBUG);
  printInfoln("                 ***",DEBUG);
  printInfo(F("*** MODELO:                             "),DEBUG); // Imprime el MODELO DEL MODULO
  printInfo(modem_info,DEBUG);
  printInfoln("             ***",DEBUG);
  printInfo(F("*** IDENTIDAD DE LA REGLETA:               "),DEBUG); // Imprime la ID
  printInfo(config.ID_regleta,DEBUG);
  printInfoln("             ***",DEBUG);
  printInfo(F("*** SERVIDOR MQTT:                      "),DEBUG); // Imprime la dirección IP MQTT
  printInfo(config.broker,DEBUG);
  printInfoln("                 ***",DEBUG);
  printInfo(F("*** PUERTO MQTT:                        "),DEBUG); // Imprime el puerto MQTT
  printInfo((String)config.port_mqtt,DEBUG);
  printInfoln("                           ***",DEBUG);
  printInfoln(F("***************************************************************************"),DEBUG);
  printInfoln("",DEBUG);
  printInfoln("",DEBUG);
  printInfoln("",DEBUG);
}
