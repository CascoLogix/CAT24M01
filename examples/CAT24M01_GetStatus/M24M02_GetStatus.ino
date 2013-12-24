// by Clint Stevenson
// Casco Logix
// @CascoLogix
// http://www.cascologix.com
// 


#include <M24M02.h>
#include <Wire.h>


M24M02 EEPROM1(0);      // Create instance of MCP23S08 with CS on pin 3

void setup() 
{
  Serial.begin(9600);
  EEPROM1.begin();      // Call begin to initialize instance
  Serial.print(EEPROM1.getStatus(), BIN);
  Serial.print("Done.");
}

void loop() 
{
  Serial.print(".");
  delay(1000);                              // Wait one second
}
