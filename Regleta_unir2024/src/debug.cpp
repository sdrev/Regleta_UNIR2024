/********************************************************************************/
/*                                                                              */
/*                            PRINTEO EN MODO DEBUG                             */
/*                                                                              */
/********************************************************************************/

#include <Arduino.h>

//Print sin salto de carro
void printInfo(String cadena, boolean debug) { 
  if (debug) {
    Serial.print(cadena);
  }
}

//Print con salto de carro
void printInfoln(String cadena, boolean debug) { 
  if (debug) {
    Serial.println(cadena);
  }
}