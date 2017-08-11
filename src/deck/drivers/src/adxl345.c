#define DEBUG_MODULE "ADXL"

#include "FreeRTOS.h"
#include "task.h"

#include "deck.h"
#include "system.h"
#include "debug.h"
#include "log.h"
#include "param.h"

#include "i2cdev.h"
#include "adxl345.h"

static uint8_t devAddr;
static I2C_Dev *I2Cx;
static bool isInit;
static uint8_t buffer[6];

void adxl345Init(DeckInfo* info)
{
	if (isInit)
		return;

	i2cdevInit(I2C1_DEV);
  	I2Cx = I2C1_DEV;
  	devAddr = ADXL345_DEFAULT_ADDRESS;
  	isInit = true;

  	DEBUG_PRINT("ADXL345 Driver Initalize!\n");


} 

bool adxlTest()
{
	bool testStatus;

  	if (!isInit)
    	return false;

    testStatus = adxlTestConnection();
    DEBUG_PRINT("ADXL345 Test Complete! \n");

    return testStatus;
}

bool adxlTestConnection()
{
	return adxlGetDeviceID() == 0xE5;//0b11100101;	
}

uint8_t adxlGetDeviceID()
{
	i2cdevReadByte(I2Cx,devAddr, ADXL345_RA_DEVID, buffer);
	return buffer[0];	
}

void adxlgetAcceleration(int16_t* x, int16_t* y, int16_t* z)
{
	i2cdevRead(I2Cx,devAddr, ADXL345_RA_DATAX0, 6, buffer);
    *x = (((int16_t)buffer[1]) << 8) | buffer[0];
    *y = (((int16_t)buffer[3]) << 8) | buffer[2];
    *z = (((int16_t)buffer[5]) << 8) | buffer[4];	
    DEBUG_PRINT("X %d, \t Y %d, \t Z %d \n",(int)x,(int)y,(int)z);	
}


static const DeckDriver adxl345_deck = {
  .name = "adxl345Driver",
  .init = adxl345Init,
  .test = adxlTest,
};

DECK_DRIVER(adxl345_deck);