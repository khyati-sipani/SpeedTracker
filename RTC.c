
#include "Driver_I2C.h"
#include "RTE_Components.h"
#include "RTC.h"

#ifdef RTE_CMSIS_RTOS
#include "cmsis_os.h"
#endif

#ifndef RTC_I2C_PORT
#define RTC_I2C_PORT       0
#endif

#define RTC_I2C_ADDR_READ       0xD1
#define RTC_I2C_ADDR_WRITE 			0xD0

//#define FXOS8700_ID             0xC7

/* I2C Driver */
#define _I2C_Driver_(n)  Driver_I2C##n
#define  I2C_Driver_(n) _I2C_Driver_(n)
extern ARM_DRIVER_I2C    I2C_Driver_(RTC_I2C_PORT);
#define ptrI2C         (&I2C_Driver_(RTC_I2C_PORT))

/* Register addresses */
#define REG_SEC               	0x00
#define REG_MINUTE            	0x01
#define REG_HOUR              	0x02
#define REG_DAY               	0x03
#define REG_DATE               	0x04
#define REG_MONTH               0x05
#define REG_YEAR								0x06

/* Driver status */
#define STATUS_INIT             0x01
static int8_t Status;

int32_t read_RTC(uint8_t * array_ptr){
	uint8_t data[7];
	data[0] = REG_SEC;
	ptrI2C->MasterTransmit(RTC_I2C_ADDR_READ, &data[0], 1, true);
	while (ptrI2C->GetStatus().busy);
	if (ptrI2C->GetDataCount() != 1) { return -1; }
  ptrI2C->MasterReceive (RTC_I2C_ADDR_READ, array_ptr, 7, false);
  while (ptrI2C->GetStatus().busy);
  if (ptrI2C->GetDataCount() != 7) { return -1; }

  return 0;
}

int32_t set_clock() {
  uint8_t data[8];

  data[0] = REG_SEC;
  data[1] = 0x80;
	data[2] = 0x01;
	data[3] = 0x01;
	data[4] = 0x01;
	data[5] = 0x01;
	data[6] = 0x01;
	data[7] = 0x03;
	
  ptrI2C->MasterTransmit(RTC_I2C_ADDR_WRITE, data, 8, false);
  while (ptrI2C->GetStatus().busy);
  if (ptrI2C->GetDataCount() != 8) { return -1; }
  return 0;
}

