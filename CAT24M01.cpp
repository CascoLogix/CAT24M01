


#include "Arduino.h"
#include <Wire.h>
#include "CAT24M01.h"


#define DEFAULT_ADDRESS		0x0A	// From CAT24M01 Datasheet

//#define DEBUG_CAT24M01			// Uncomment to get serial UART debug strings

/***********************************************************************************
 * 
 * Class method name: CAT24M01(uint8_t I2Caddress)
 * 
 * Scope: public
 * 
 * Parameters:
 * 		I2Caddress: This is the I2C bus address
 *
 * Returns:
 *		None
 * 
 * Description:
 * 		This is the CAT24M01 class constructor.  The CAT24M01 device address gets
 * stored in a private class variable to be used throughout the class methods.
 * 
 **********************************************************************************/
CAT24M01::CAT24M01(uint8_t I2Caddress)
{
	this->busAddress = I2Caddress;
}


/***********************************************************************************
 * 
 * Class method name: begin()
 * 
 * Scope: public
 * 
 * Parameters:
 * 		None
 *
 * Returns:
 *		None
 * 
 * Description:
 * 		This calls the Wire.begin() class method to initialize the I2C interface.
 * 
 **********************************************************************************/
void CAT24M01::begin()
{  
	Wire.begin();        					// Join I2C bus
}


/***********************************************************************************
 * 
 * Class method name: write(uint32_t memAddress, uint8_t data)
 * 
 * Scope: public
 * 
 * Parameters:
 * 		memAddress: This is the internal EEPROM memory address being accessed. This 
 *			device supports 17-bit address space, so a uint32_t type is used.
 *		data: This is the data being sent to the EEPROM to be written into NVM
 *
 * Returns:
 *		Returns 1 if the byte was written, or 0 if there was an error
 * 
 * Description:
 * 		This method makes calls to various Wire class methods to perform the 
 * necessary steps to perform a single byte write.
 * 
 **********************************************************************************/
uint32_t CAT24M01::write(uint32_t memAddress, uint8_t data)
{
	uint8_t retVal;
	
	// First transmission byte (beginTransmission) needs bits aligned per 24M01 datasheet
	// Wire library adds the R/W bit, so don't left shift the bits for the R/W bit here
	// 16th bit of memory address gets stuffed in here, then send next two octets of memory address bits
    Wire.beginTransmission((uint8_t)((DEFAULT_ADDRESS << 3) | ((this->busAddress) << 1) | ((memAddress >> 16) & 0x01)));
    Wire.write((uint8_t)((memAddress >> 8) & 0xFF));// Send memory address bits [15:8]
    Wire.write((uint8_t)(memAddress & 0xFF)); 		// Send memory address bits [7:0]
    Wire.write(data);								// Send data byte to write
    retVal = Wire.endTransmission();				// End I2C transmission
	
#ifdef DEBUG_CAT24M01
	if (retVal)
	{
		Serial.print("Write error: ");
		Serial.println(retVal);
	}
#endif // DEBUG_CAT24M01
	
	return retVal;									// Return number 1 for success; 0 for error
}


/***********************************************************************************
 * 
 * Class method name: write(uint32_t memAddress, uint8_t * buffer, uint8_t numBytes)
 * 
 * Scope: public
 * 
 * Parameters:
 * 		memAddress: This is the internal EEPROM memory address being accessed. This 
 *			device supports 17-bit address space, so a uint32_t type is used.
 *		buffer: This is a buffer containing data being to be sent to the EEPROM to 
 *			write to NVM
 *		numBytes: This is the number of bytes to write to NVM
 *
 * Returns:
 *		Returns the number of bytes written, or 0 if there was an error
 * 
 * Description:
 * 		This method makes calls to various Wire class methods to perform the 
 * necessary steps to perform a block write.
 * 
 **********************************************************************************/
uint32_t CAT24M01::write(uint32_t memAddress, uint8_t * buffer, uint8_t numBytes) 
{
	uint8_t retVal;
	
	// First transmission byte (beginTransmission) needs bits aligned per 24M01 datasheet
	// Wire library adds the R/W bit, so don't left shift the bits for the R/W bit here
	// 16th bit of memory address gets stuffed in here, then send next two octets of memory address bits
    Wire.beginTransmission((uint8_t)((DEFAULT_ADDRESS << 3) | ((this->busAddress) << 1) | ((memAddress >> 16) & 0x01)));
    Wire.write((uint8_t)((memAddress >> 8) & 0xFF));// Send memory address bits [15:8]
    Wire.write((uint8_t)(memAddress & 0xFF));		// Send memory address bits [7:0]

	uint8_t bytesWritten;
	bytesWritten = Wire.write(buffer, numBytes);	// Send buffer of data bytes to write
    retVal = Wire.endTransmission();				// End I2C transmission
	
#ifdef DEBUG_CAT24M01
	if (retVal)
	{
		Serial.print("Write error: ");
		Serial.println(retVal);
	}
#endif // DEBUG_CAT24M01
	
	return (uint32_t)bytesWritten;					// Return number of bytes written
}
  
  
/***********************************************************************************
 * 
 * Class method name: read(uint32_t memAddress, uint8_t * buffer)
 * 
 * Scope: public
 * 
 * Parameters:
 * 		memAddress: This is the internal EEPROM memory address being accessed. This 
 *			device supports 17-bit address space, so a uint32_t type is used.
 *		buffer: This is a buffer for receiving the data being read from the EEPROM to
 *			write to NVM
 *
 * Returns:
 *		Returns the number of bytes read, or 0 if there was an error
 * 
 * Description:
 * 		This method makes calls to various Wire class methods to perform the 
 * necessary steps to perform a single byte read.
 * 
 **********************************************************************************/
uint32_t CAT24M01::read(uint32_t memAddress, uint8_t * buffer)
{
	uint8_t retVal;
	
	deviceSelect(memAddress);						// Perform device select command
	
	Wire.write((uint8_t)((memAddress >> 8) & 0xFF));// Send memory address bits [15:8]
    Wire.write((uint8_t)(memAddress & 0xFF));		// Send memory address bits [7:0]
	
	retVal = Wire.endTransmission(0);				// Send restart condition (false)

#ifdef DEBUG_CAT24M01
	if (retVal)
	{
		Serial.print("No ACK from endTransmission. Err num = ");
		Serial.println(retVal);
	}
#endif // DEBUG_CAT24M01

	retVal = initiateRead(memAddress, 1);			// Initiate read operation
	
#ifdef DEBUG_CAT24M01
	if (retVal)
	{
		Serial.print("Read single byte error: ");
		Serial.println(retVal);
	}
#endif // DEBUG_CAT24M01
	
    if (Wire.available()) 
	{
		*buffer = Wire.read();						// Read bytes from I2C receive buffer
	}
	
#ifdef DEBUG_CAT24M01
	else
	{
		Serial.println("I2C read buffer empty");
	}
#endif // DEBUG_CAT24M01

    return 1;										// Return number of bytes read (always 1 here)
}


/***********************************************************************************
 * 
 * Class method name: read(uint32_t memAddress, uint8_t * buffer, uint8_t numBytes)
 * 
 * Scope: public
 * 
 * Parameters:
 * 		memAddress: This is the internal EEPROM memory address being accessed. This 
 *			device supports 17-bit address space, so a uint32_t type is used.
 *		buffer: This is a buffer for receiving the data being read from the EEPROM to
 *			write to NVM
 *		numBytes: This is the number of bytes to read from NVM
 *
 * Returns:
 *		Returns the number of bytes read, or 0 if there was an error
 * 
 * Description:
 * 		This method makes calls to various Wire class methods to perform the 
 * necessary steps to perform a block read.
 * 
 **********************************************************************************/
uint32_t CAT24M01::read(uint32_t memAddress, uint8_t * buffer, uint8_t numBytes)
{
	uint8_t retVal;
	
	deviceSelect(memAddress);						// Perform device select command
	
	Wire.write((uint8_t)((memAddress >> 8) & 0xFF));// Send memory address bits [15:8]
    Wire.write((uint8_t)(memAddress & 0xFF));		// Send memory address bits [7:0]
	
	retVal = Wire.endTransmission(0);				// Send restart condition (false)

#ifdef DEBUG_CAT24M01
	if (retVal)
	{
		Serial.print("NACK from endTransmission. Err num = ");
		Serial.println(retVal);
	}
#endif // DEBUG_CAT24M01

	retVal = initiateRead(memAddress, numBytes);	// Initiate read operation
	
#ifdef DEBUG_CAT24M01
	if (retVal)
	{
		Serial.print("Read multiple bytes error: ");
		Serial.println(retVal);
	}
#endif // DEBUG_CAT24M01
	
    uint32_t index;
    for (index = 0; index < numBytes; index++ )
	{
		if (Wire.available())
		{
			buffer[index] = Wire.read();			// Read bytes from I2C receive buffer
		}
		
#ifdef DEBUG_CAT24M01
		else
		{
			Serial.println("I2C read buffer empty");
		}
#endif // DEBUG_CAT24M01
	}
	
	return index;									// Return number of bytes read
}

  
/***********************************************************************************
 * 
 * Class method name: getStatus(uint32_t memAddress)
 * 
 * Scope: public
 * 
 * Parameters:
 * 		memAddress: This is the internal EEPROM memory address being accessed. This 
 *			device supports 17-bit address space, so a uint32_t type is used. This
 *			is only needed for the 17th bit, which is needed to perform the device
 *			select command.
 *
 * Returns:
 *		Returns the busy status of the CAT24M01 chip
 * 
 * Description:
 * 		This method makes calls to various Wire class methods to perform the 
 * necessary steps to perform a status read. The status can be polled as an 
 * alternative option to fixed time wait after a write operation.
 * 
 **********************************************************************************/
uint8_t CAT24M01::getStatus(void)
{
	uint8_t retVal = 0;
	
	// First transmission byte (beginTransmission) needs bits aligned per 24M01 datasheet
	// Wire library adds the R/W bit, so don't left shift the bits for the R/W bit here
	// 16th bit of memory address gets stuffed in here, then send next two octets of memory address bits
    Wire.beginTransmission((uint8_t)((DEFAULT_ADDRESS << 3) | ((this->busAddress) << 1)));
	retVal = Wire.endTransmission();
	
#ifdef DEBUG_CAT24M01
	if (retVal)
	{
		Serial.print("Get status error: ");
		Serial.println(retVal);
	}
#endif // DEBUG_CAT24M01

	return retVal;									// Return busy status
}


/***********************************************************************************
 * 
 * Class method name: deviceSelect(uint32_t memAddress)
 * 
 * Scope: private
 * 
 * Parameters:
 * 		memAddress: This is the internal EEPROM memory address being accessed. This 
 *			device supports 17-bit address space, so a uint32_t type is used. This
 *			is only needed for the 17th bit, which is needed to perform the device
 *			select command.
 *
 * Returns:
 *		None
 * 
 * Description:
 * 		This method makes calls to various Wire class methods to perform the 
 * necessary steps to perform a device select command, which is required prior to 
 * any other operation on the CAT24M01 chip. This function is used by the other 
 * methods in this class. Last, this function call must be followed by the remaining
 * memory address bits if the intent is to write data, but must always be followed 
 * by an endTransmission() call in order to free the I2C bus.
 * 
 **********************************************************************************/
void CAT24M01::deviceSelect(uint32_t memAddress)	
{
	// First transmission byte (beginTransmission) needs bits aligned per 24M01 datasheet
	// Wire library adds the R/W bit, so don't left shift the bits for the R/W bit here
	// 16th bit of memory address gets stuffed in here, then send next two octets of memory address bits
    Wire.beginTransmission((uint8_t)((DEFAULT_ADDRESS << 3) | ((this->busAddress) << 1) | ((memAddress >> 16) & 0x01)));
}


/***********************************************************************************
 * 
 * Class method name: initiateRead(uint32_t memAddress, uint8_t numBytes)
 * 
 * Scope: private
 * 
 * Parameters:
 * 		memAddress: This is the internal EEPROM memory address being accessed. This 
 *			device supports 17-bit address space, so a uint32_t type is used. This
 *			is only needed for the 17th bit, which is needed to perform the device
 *			select command.
 *		numBytes: This is the number of bytes to read from CAT24M01 chip
 *
 * Returns:
 *		Returns the error status of the beginTransmission() method
 * 
 * Description:
 * 		This method makes calls to various Wire class methods to perform the 
 * necessary steps to initiate a read operation. This function is used by the other 
 * methods in this class. The bytes read back during the Wire.requestFrom() method 
 * are stored in a buffer internal to the Wire class. Data is later accessed from 
 * the read buffer by calling the Wire.read() method. 
 * 
 **********************************************************************************/
uint8_t CAT24M01::initiateRead(uint32_t memAddress, uint8_t numBytes)
{
	uint8_t retVal;
	
	// First transmission byte (requestFrom) needs bits aligned per 24M01 datasheet
	// Wire library adds the R/W bit, so don't left shift the bits for the R/W bit here
	// 16th bit of memory address gets stuffed in here, then send next two octets of memory address bits
	// requestFrom() puts the received bytes in a receive buffer.
    retVal = Wire.requestFrom(((DEFAULT_ADDRESS << 3) | ((this->busAddress) << 1) | ((memAddress >> 16) & 0x01)), (uint8_t)numBytes);
	
#ifdef DEBUG_CAT24M01
	if (0 == retVal)
	{
		Serial.print("Initiate read error: ");
		Serial.println(retVal);
	}
#endif // DEBUG_CAT24M01

	return retVal;									// Return number of bytes read
}
