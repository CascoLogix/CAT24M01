// by Clint Stevenson
// Casco Logix
// @CascoLogix
// http://www.cascologix.com
// 


#include <M24M01.h>
#include <Wire.h>


M24M01 EEPROM;

void setup() 
{
  Serial.begin(9600);
  EEPROM.begin();      // Call begin to initialize instance
  Serial.print(EEPROM.getStatus(), BIN);
  Serial.print("Done.");
}

void loop() 
{
  Serial.print(".");
  delay(1000);                              // Wait one second
}
