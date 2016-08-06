// by Clint Stevenson
// Casco Logix
// @CascoLogix
// http://www.cascologix.com
// 


#include <M24M01.h>
#include <Wire.h>

const char str1[] = "Hello from M24M01";

char readStr[64];

M24M01 EEPROM;

void setup() 
{
  Serial.begin(9600);
  
  EEPROM.begin();      // Call begin to initialize instance
  
  Serial.print("str1 length is: ");
  Serial.println(strlen(str1));
  Serial.print(EEPROM.write(0, (uint8_t*)str1, strlen(str1)));
  Serial.println(" bytes written.");
  
  Serial.print(EEPROM.read(0, (uint8_t*)readStr, 64));
  Serial.println(" bytes read.");
  Serial.println(readStr);

  Serial.print("Done.");
}

void loop() 
{
  Serial.print(".");
  delay(1000);                              // Wait one second
}
