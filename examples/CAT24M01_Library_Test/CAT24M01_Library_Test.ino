// by Clint Stevenson
// Casco Logix
// @CascoLogix
// http://www.cascologix.com


#include <CAT24M01.h>

const char constStr[] = "Hello from M24M01";
const uint8_t constDataArray[] = {13, 14, 15, 16, 17, 18, 19, 20};

char readStr[sizeof(constStr) + 1];
uint8_t dataArray[sizeof(constDataArray)];

CAT24M01 EEPROM1(0x00);     

void setup() 
{
  Serial.begin(9600);
  Serial.println("Ready");

  EEPROM1.begin();     

  readStatus();
  writeString(0x00, constStr);
  readString(0x00, readStr, strlen(constStr));
  singleByteWrite(0x100, 0xCC);
  singleByteRead(0x100);
  multiByteWrite(0x200, constDataArray, sizeof(constDataArray));
  multiByteRead(0x200, dataArray, sizeof(dataArray));
  dumpMemoryRange(0x00, 0x2FF);
  //dumpMemoryAll();
  Serial.println("Library function testing complete.");
}


void loop() 
{
}


void writeString (uint32_t memoryAddress, char * strBuf)
{
  uint32_t numBytes;
  
  Serial.println("Writing string to random address.");
  Serial.println("Address to write: ");
  Serial.println(memoryAddress, HEX);
  Serial.println("String to write:");
  Serial.println(strBuf);
  numBytes = EEPROM1.write(memoryAddress, strBuf, strlen(strBuf));
  numBytes += EEPROM1.write(memoryAddress + strlen(strBuf), 0);
  
  uint8_t loopCount;
  for(loopCount = 0; loopCount < 100; loopCount++)
  {
    if(0 != EEPROM1.getStatus())
    {
      Serial.println("Busy");
      delay(200);
    }
  }

  Serial.print(numBytes);
  Serial.println(" bytes written.");
  Serial.println("writeString done.");
  Serial.println();
}


void readString (uint32_t memoryAddress, char * strBuf, uint8_t numBytes)
{
  Serial.println("Reading string from random address.");
  Serial.println("Location of string: ");
  Serial.println(memoryAddress, HEX);
  Serial.println("Expected string length: ");
  Serial.println(numBytes);
  numBytes = EEPROM1.read(memoryAddress, strBuf, numBytes);
  Serial.print(numBytes);
  Serial.println(" bytes read.");
  Serial.println("String read: ");
  Serial.println(strBuf);
  Serial.println("readString done.");
  Serial.println();
}


void singleByteWrite (uint32_t memoryAddress, uint8_t dataValue)
{
  Serial.println("Writing single byte to random address.");
  Serial.println("Address to write: ");
  Serial.println(memoryAddress, HEX);
  Serial.println("Value to write to memory: ");
  Serial.println(dataValue, HEX);
  EEPROM1.write(memoryAddress, dataValue);
  
  uint8_t loopCount;
  for(loopCount = 0; loopCount < 100; loopCount++)
  {
    if(0 != EEPROM1.getStatus())
    {
      Serial.println("Busy");
      delay(200);
    }
  }

  Serial.println("singleByteWrite done.");
  Serial.println();
}


void singleByteRead (uint32_t memoryAddress)
{
  uint8_t dataValue;
  
  Serial.println("Reading single byte from random address.");
  Serial.println("Address to read: ");
  Serial.println(memoryAddress, HEX);
  EEPROM1.read(memoryAddress, &dataValue);
  Serial.println("Value read from memory: ");
  Serial.println(dataValue, HEX);
  Serial.println("singleByteRead done.");
  Serial.println();
}


void dumpMemoryAll (void)
{
  dumpMemoryRange(0, 0x1FFFF);
}


void dumpMemoryRange (uint32_t memoryStartAddress, uint32_t memoryEndAddress)
{
  uint32_t currentAddress;
  uint8_t columns;
  uint8_t val;
  
  Serial.println("dumpMemoryRange begin.");
  Serial.print("Start address: ");
  Serial.println(memoryStartAddress, HEX);
  columns = 0;
  for(currentAddress = memoryStartAddress; currentAddress <= memoryEndAddress; currentAddress++)
  {
    EEPROM1.read(currentAddress, &val);
    if(val < 0x10)
    {
      Serial.print('0');
    }
    
    Serial.print(val, HEX);
    Serial.print(" ");
    
    columns++;

    if(columns > 7)
    {
      Serial.println();
      columns = 0;
    }
  }
  Serial.print("End address: ");
  Serial.println(memoryEndAddress, HEX);
  Serial.println("dumpMemoryRange done.");
  Serial.println();
}


void multiByteWrite (uint32_t memoryAddress, uint8_t * dataBuffer, uint8_t numBytes)
{
  Serial.println("Writing multiple bytes starting at random address.");
  Serial.println("Begin address: ");
  Serial.println(memoryAddress, HEX);
  Serial.println("End address: ");
  Serial.println((memoryAddress + numBytes - 1), HEX);
  EEPROM1.write(memoryAddress, dataBuffer, numBytes);
  Serial.println("multiByteWrite done.");
  Serial.println();
}


void multiByteRead (uint32_t memoryAddress, uint8_t * dataBuffer, uint32_t numBytes)
{
  Serial.println("Reading multiple bytes starting at random address.");
  Serial.println("Begin address: ");
  Serial.println(memoryAddress, HEX);
  EEPROM1.read(memoryAddress, dataBuffer, numBytes);
  Serial.println("Data read from memory: ");

  uint8_t idx;
  for(idx = 0; idx < numBytes; idx++)
  {
    if(dataBuffer[idx] < 0x10)
    {
      Serial.print('0');
    }
    Serial.print(dataBuffer[idx], HEX);
    Serial.print(' ');
  }

  Serial.println();
  Serial.println("multiByteRead done.");
  Serial.println();
}


void readStatus (void)
{
  uint8_t busyStatus;
  
  Serial.println("Checking busy status...");
  
  busyStatus = EEPROM1.getStatus();

  Serial.print("Status = ");
  
  if(0 == busyStatus)
  {
    Serial.println("Not busy");

    Serial.println("Writing byte value 0xAA at memory address 0xFF");
    EEPROM1.write(0xFF, 0xAA);
    Serial.println("Checking busy status...");
    
    busyStatus = EEPROM1.getStatus();
  
    Serial.print("Status = ");
    
    if(0 == busyStatus)
    {
      Serial.println("Not busy");
    }
    else
    {
      Serial.println("Busy");
    }
  }
  
  else
  {
    Serial.println("Busy. Try again later.");
  }
  
  Serial.println("Done checking busy status.");
  Serial.println();
}


