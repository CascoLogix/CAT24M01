


#include "Arduino.h"
#include <Wire.h>
#include "CAT24M01.h"


#define DEFAULT_ADDRESS		0x0A


// Constructor
CAT24M01::CAT24M01(uint8_t address)
{
	this->busAddress = address;
}


void CAT24M01::begin()						// Initialize interfaces
{  
	Wire.begin();        					// Join I2C bus
}


uint32_t CAT24M01::write(uint32_t address, uint8_t data)			// Write a byte to EEPROM
{
	uint8_t errNo;
	
    Wire.beginTransmission((uint8_t)((DEFAULT_ADDRESS << 3) | ((this->busAddress) << 1) | ((address >> 16) & 0x01)));
    Wire.write((uint8_t)((address >> 8) & 0xFF));	// MSB
    Wire.write((uint8_t)(address & 0xFF)); 	// LSB
    Wire.write(data);
    errNo = Wire.endTransmission();
	
	if (errNo)
	{
		Serial.print("Write error: ");
		Serial.println(errNo);
	}
	
	return 1;
}


uint32_t CAT24M01::write(uint32_t address, uint8_t * buffer, uint8_t numBytes) 
{
	uint8_t errNo;
	
    Wire.beginTransmission((uint8_t)((DEFAULT_ADDRESS << 3) | ((this->busAddress) << 1) | ((address >> 16) & 0x01)));
    Wire.write((uint8_t)((address >> 8) & 0xFF));	// MSB
    Wire.write((uint8_t)(address & 0xFF)); 	// LSB
    
	uint8_t bytesWritten;
	bytesWritten = Wire.write(buffer, numBytes);
    errNo = Wire.endTransmission();
	
	if (errNo)
	{
		Serial.print("Write error: ");
		Serial.println(errNo);
	}
	
	return (uint32_t)bytesWritten;
	
	/*
	uint32_t index;
    for (index = 0; index < numBytes; index++)
    {
		Wire.write(buffer[index]);
		//Serial.write(buffer[index]);
	}
	
    Wire.endTransmission();
	
	return index;
	*/
}
  
  
uint32_t CAT24M01::read(uint32_t address, uint8_t * buffer)	// Read a byte from EEPROM
{
	uint8_t errNo;
	
    Wire.beginTransmission((uint8_t)((DEFAULT_ADDRESS << 3) | ((this->busAddress) << 1) | ((address >> 16) & 0x01)));
    Wire.write((uint8_t)((address >> 8) & 0xFF));	// MSB
    Wire.write((uint8_t)(address & 0xFF)); 	// LSB
    errNo = Wire.endTransmission();
	
	if (errNo)
	{
		Serial.print("Read error: ");
		Serial.println(errNo);
	}
	
    Wire.requestFrom((uint8_t)((DEFAULT_ADDRESS << 3) | ((this->busAddress) << 1) | ((address >> 16) & 0x01)), (uint8_t)1);
	
    if (Wire.available()) 
	{
		*buffer = Wire.read();
	}
	
    return 1;
}


uint32_t CAT24M01::read(uint32_t address, uint8_t * buffer, uint8_t numBytes)
{
	uint8_t errNo;
	
    Wire.beginTransmission((uint8_t)((DEFAULT_ADDRESS << 3) | ((this->busAddress) << 1) | ((address >> 16) & 0x01)));
    Wire.write((uint8_t)((address >> 8) & 0xFF));	// MSB
    Wire.write((uint8_t)(address & 0xFF)); 	// LSB
    errNo = Wire.endTransmission();
	
	if (errNo)
	{
		Serial.print("Read error: ");
		Serial.println(errNo);
	}
	
    Wire.requestFrom((int)this->busAddress, (int)numBytes);
	
    uint32_t index;
    for (index = 0; index < numBytes; index++ )
	{
		if (Wire.available())
		{
			buffer[index] = Wire.read();
		}
	}
	
	return index;
}

  
uint8_t CAT24M01::getStatus(uint32_t address)				// Read the EEPROM status 
{
	uint8_t retVal = 0;
	
    Wire.beginTransmission((uint8_t)((DEFAULT_ADDRESS << 3) | ((this->busAddress) << 1) | ((address >> 16) & 0x01)));
	retVal = Wire.endTransmission();
	
	return retVal;
}
