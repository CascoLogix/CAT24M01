

#ifndef CAT24M01_H
#define CAT24M01_H


#include <inttypes.h>


#define CAT24M01_PAGE_SIZE			(256)
#define CAT24M01_NUM_PAGES			(512)
#define CAT24M01_CAPACITY_BYTES		(CAT24M01_PAGE_SIZE * CAT24M01_NUM_PAGES)


class CAT24M01 {
public:
	CAT24M01(uint8_t I2Caddress);			// Constructor
	
	void begin();							// Initialize interfaces
	
	uint32_t write(uint32_t memAddress, 	// Write a byte to EEPROM
					uint8_t data);
	uint32_t write(uint32_t memAddress, 	// Write a byte to EEPROM
					uint8_t * buffer, 
					uint8_t numBytes);
	uint32_t read(uint32_t memAddress, 		// Read a byte from EEPROM
					uint8_t * buffer);
	uint32_t read(uint32_t memAddress, 		// Read a byte from EEPROM
					uint8_t * buffer, 
					uint8_t numBytes);
	uint8_t getStatus(void);				// Read the EEPROM status register

private:
	uint8_t busAddress;						// I2C Device Address
	void deviceSelect(uint32_t memAddress);	// Select CAT24M01 chip
	uint8_t initiateRead(uint32_t memAddress, uint8_t numBytes);	// Initiate read
};

#endif // CAT24M01_H