// by Clint Stevenson
// Casco Logix
// @CascoLogix
// http://www.cascologix.com
// 


#include <M24M02.h>
#include <Wire.h>

const char str1[] = "Hello from M24M02";

char readStr[64];

M24M02 EEPROM1(0);      // Create instance of MCP23S08 with CS on pin 3

void setup() 
{
  Serial.begin(9600);
  
  EEPROM1.begin();      // Call begin to initialize instance
  
  Serial.print("str1 length is: ");
  Serial.println(strlen(str1));
  Serial.print(EEPROM1.write(0, (uint8_t*)str1, strlen(str1)));
  Serial.println(" bytes written.");
  
  Serial.print(EEPROM1.read(0, (uint8_t*)readStr, 64));
  Serial.println(" bytes read.");
  Serial.println(readStr);

  Serial.print("Done.");
}

void loop() 
{
  Serial.print(".");
  delay(1000);                              // Wait one second
}
