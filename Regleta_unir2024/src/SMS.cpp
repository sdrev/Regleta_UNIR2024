/********************************************************************************/
/*                                                                              */
/*                       RUTINA PARA RECEPCION DE SMSs                          */
/*                              E.D.O. 2023                                     */
/*                              Version 2.0                                     */
/*                                                                              */
/********************************************************************************/

#include "Relgeta.h"
#include "Adafruit_FONA.h"
#include "TinyGSM.h"

// VARIABLES
String smsString = "";
char smsBuffer[250];

extern regleta_config_t config;
extern boolean DEBUG;

extern TinyGsm modem;
extern Adafruit_FONA sim7000;


// RECEPCION DE SMS <-COMPRUEBA SI HAY SMSs, LOS PROCESA Y LOS BORRA
void recibirSMS()
{
  char sms_data[256], sms_sender[32];
  uint16_t sms_len;

  Serial.print(millis());
   printInfoln(" RUTINA LECTURA DE SMS",DEBUG);

  if (!modem.isNetworkConnected())
  {
    printInfoln("Red NO conectada",DEBUG);
    sim7000_INICIO();
  }

  if (sim7000.getNumSMS() > 0)
  {
    printInfo("SMS disponibles módem: ",DEBUG);
    printInfoln((String)(sim7000.getNumSMS()), DEBUG);

    for (uint8_t i = 0; i < sim7000.getNumSMS(); i++)
    {
      sim7000.readSMS(i, sms_data, 128, &sms_len);
      sim7000.getSMSSender(i, sms_sender, 32);
      printInfo("DATOS DEL SMS " + (String)i + " es: " + (String)sms_data +". Enviado del numero: " +(String)sms_sender ,DEBUG);
      processSMS(String(sms_data), sms_sender);
    }
    for (uint8_t i = 0; i < 20; i++)
      delSMS(i); // Borramos todos los SMS una vez procesados
  }
}


// RECEPCION DE SMS <----- ANALIZA EL SMS RECIBIDO Y REALIZA ACCION
void processSMS(String smsString, char *callerIDbuffer)
{
  printInfoln(" PROCESANDO COMANDO " + smsString + " DE " + String(callerIDbuffer),DEBUG);
  //*****************************************************************
  //ACTIVAR TODAS LAS SALIDAS
  //*****************************************************************
  if (smsString == "ACTIVAR TODAS LAS SALIDAS")
  {
    printInfoln("Activando Salidas", DEBUG);
    digitalWrite(RELAY1, 1);
    digitalWrite(RELAY2, 1);
    digitalWrite(RELAY3, 1);
    mqttEstadoActual();
    delay(1000);

    if (!modem.sendSMS(callerIDbuffer, "TODAS SALIDAS ACTIVADAS."))
    {
      printInfoln(F("Failed"),DEBUG);
    }
    else
    {
      printInfoln(F("Sent!"),DEBUG);
    }
  }
  //*****************************************************************
  //DESACTIVAR TODAS LAS SALIDAS
  //*****************************************************************
  else if (smsString == "DESACTIVAR TODAS LAS SALIDAS")
  {
    printInfoln("Desactivando Salidas", DEBUG);
    digitalWrite(RELAY1, 0);
    digitalWrite(RELAY2, 0);
    digitalWrite(RELAY3, 0);
    mqttEstadoActual();
    delay(1000);

    // modem.sendSMS(config.tlfn_jefe_equipo, "TODAS SALIDAS DESACTIVADAS.");
    if (!modem.sendSMS(callerIDbuffer, "TODAS SALIDAS DESACTIVADAS."))
    {
      printInfoln(F("Failed"), DEBUG);
    }
    else
    {
      printInfoln(F("Sent!"), DEBUG);
    }
  }
  //*****************************************************************
  //ACTIVAR/DESACTIVAR RELAY 1
  //*****************************************************************
  else if (smsString == "ACTIVAR SALIDA1")
  {
    printInfoln("ACTIVANDO SALIDA1.", DEBUG);
    digitalWrite(RELAY1, 1);
    mqttEstadoActual();
    delay(1000);

    if (!modem.sendSMS(callerIDbuffer, "SALIDA1 ACTIVADA."))
    {
      printInfoln(F("Failed"), DEBUG);
    }
    else
    {
      printInfoln(F("Sent!"), DEBUG);
    }
  }
  else if (smsString == "DESACTIVAR SALIDA1")
  {
    printInfoln("DESACTIVANDO SALIDA1.", DEBUG);
    digitalWrite(RELAY1, 0);
    mqttEstadoActual();
    delay(1000);

    if (!modem.sendSMS(callerIDbuffer, "SALIDA1 DESACTIVADA."))
    {
      printInfoln(F("Failed"), DEBUG);
    }
    else
    {
      printInfoln(F("Sent!"), DEBUG);
    }
  }
  //*****************************************************************
  //ACTIVAR/DESACTIVAR RELAY 2
  //*****************************************************************
  else if (smsString == "ACTIVAR SALIDA2")
  {
    printInfoln("ACTIVANDO SALIDA2.", DEBUG);
    digitalWrite(RELAY2, 1);
    mqttEstadoActual();
    delay(1000);

    if (!modem.sendSMS(callerIDbuffer, "SALIDA2 ACTIVADA."))
    {
      printInfoln(F("Failed"), DEBUG);
    }
    else
    {
      printInfoln(F("Sent!"), DEBUG);
    }
  }
  else if (smsString == "DESACTIVAR SALIDA2")
  {
    printInfoln("DESACTIVANDO SALIDA2.", DEBUG);
    digitalWrite(RELAY2, 0);
    mqttEstadoActual();
    delay(1000);

    if (!modem.sendSMS(callerIDbuffer, "SALIDA2 DESACTIVADA."))
    {
      printInfoln(F("Failed"), DEBUG);
    }
    else
    {
      printInfoln(F("Sent!"), DEBUG);
    }
  }
  //*****************************************************************
  //ACTIVAR/DESACTIVAR RELAY 3
  //*****************************************************************
  else if (smsString == "ACTIVAR SALIDA3")
  {
    printInfoln("ACTIVANDO SALIDA3.", DEBUG);
    digitalWrite(RELAY3, 1);
    mqttEstadoActual();
    delay(1000);

    if (!modem.sendSMS(callerIDbuffer, "SALIDA3 ACTIVADA."))
    {
      printInfoln(F("Failed"), DEBUG);
    }
    else
    {
      printInfoln(F("Sent!"), DEBUG);
    }
  }
  else if (smsString == "DESACTIVAR SALIDA3")
  {
    printInfoln("DESACTIVANDO SALIDA3.", DEBUG);
    digitalWrite(RELAY3, 0);
    mqttEstadoActual();
    delay(1000);

    if (!modem.sendSMS(callerIDbuffer, "SALIDA3 DESACTIVADA."))
    {
      printInfoln(F("Failed"), DEBUG);
    }
    else
    {
      printInfoln(F("Sent!"), DEBUG);
    }
  }
  //*****************************************************************
  //SOLICITAR ESTADO DE SALIDAS REGLETA
  //*****************************************************************
  else if (smsString == "ESTADO REGLETA")
  {
    String estado = "";
    printInfoln("ESTADO DE LAS SALIDAS DE LA REGLETA:", DEBUG);

    if (digitalRead(RELAY1))
    {
      estado = "SALIDA 1 ACTIVADA.\n";
      delay(1000);
    }
    else
    {
      estado = "SALIDA 1 DESACTIVADA. \n";
      delay(1000);
    }
    if (digitalRead(RELAY2))
    {
      estado += "SALIDA 2 ACTIVADA.\n";
      delay(1000);
    }
    else
    {
      estado += "SALIDA 2 DESACTIVADA. \n";
      delay(1000);
    }
    if (digitalRead(RELAY3))
    {
      estado += "SALIDA 3 ACTIVADA.";
      delay(1000);
    }
    else
    {
      estado += "SALIDA 3 DESACTIVADA.";
      delay(1000);
    }
    modem.sendSMS(callerIDbuffer, estado);
    mqttEstadoActual();
    delay(1000);
  }
  //*****************************************************************
  //RESET TOTAL
  //*****************************************************************
  else if (smsString == "RESET UN1R2024")
  {
    modem.sendSMS(callerIDbuffer, "REINICIANDO SISTEMA TOTALMENTE");
    delay(1000);
    ESP.restart();
  }
  //*****************************************************************
  //SOLICITAR COMANDOS DE LA CAJA
  //*****************************************************************
  else if (smsString == "COMANDOS")
  {
    printInfoln("COMANDOS DE LA REGLETA:", DEBUG);
    String comandos =
    comandos = "ACTIVAR TODAS LAS SALIDAS\n";
    comandos += "DESACTIVAR TODAS LAS SALIDAS\n";
    comandos += "ACTIVAR SALIDA1\n";
    comandos += "DESACTIVAR SALIDA1\n";
    comandos += "ACTIVAR SALIDA2\n";
    comandos += "DESACTIVAR SALIDA2\n";
    comandos += "ACTIVAR SALIDA3\n";
    comandos += "DESACTIVAR SALIDA3\n";
    comandos += "ESTADO REGLETA";
    modem.sendSMS(callerIDbuffer, comandos);
  }
  //*****************************************************************
  //SOLICITAR COMANDOS ADMIN CAJA
  //*****************************************************************
  else if ((smsString == "COMANDOS ADMIN UN1R2024"))
  {
    modem.sendSMS(callerIDbuffer, "RESET UN1R2024");
  }
}


// BORRADO DE SMS 
void delSMS(int slot)
{
  if (sim7000.deleteSMS(slot))
  {
    printInfoln(F("OK!"),DEBUG);
  }
  else
  {
    printInfoln(F("IMPOSIBLE BORRAR SMS DEL SLOT "),DEBUG);
    printInfoln((String)slot,DEBUG);
  }
}