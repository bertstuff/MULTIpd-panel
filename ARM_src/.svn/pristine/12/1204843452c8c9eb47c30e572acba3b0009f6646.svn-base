#ifndef	NAVMATE_I2C_DIGI
#define	NAVMATE_I2C_DIGI
#define  i2c_input	0x40 // PCF8574
#define  i2c_output	0x70 // PCF8574A
#endif

#include "I2C_8574.h"
#include <Core/Peripherals/I2C_Dev_Init.h>

void I2C_8574_Init (void){
	I2C_Dev_Init(PCF8574_MAX_SPEED_HZ);
}

Status I2C_8574_WriteOutput (uint8_t address, uint8_t* Data, uint8_t Input_Mask)
{  
   I2C_M_SETUP_Type Dev_buffer;
   
  if (address < 10) {
    Dev_buffer.sl_addr7bit = (i2c_output >> 1) + address ;
	Input_Mask = 0;
   }
  else {
    Dev_buffer.sl_addr7bit = address >> 1 ;
  }
  *Data = (*Data | Input_Mask);
  Dev_buffer.tx_data = Data;
  Dev_buffer.tx_length = 1;
  Dev_buffer.retransmissions_max = 5;
  Dev_buffer.rx_data = NULL;
  Dev_buffer.rx_length = 0;
  return I2C_MasterTransferData(LPC_I2C1, &Dev_buffer, I2C_TRANSFER_POLLING);
}


Status I2C_8574_ReadOutput (uint8_t address, uint8_t* Data)
{
   I2C_M_SETUP_Type Dev_buffer;
   
  if (address < 10) {
    Dev_buffer.sl_addr7bit = (i2c_output >> 1) + address ;
   }
  else {
    Dev_buffer.sl_addr7bit = address >> 1 ;
  }
  Dev_buffer.tx_data = NULL;
  Dev_buffer.tx_length = 0;
  Dev_buffer.retransmissions_max = 5;
  Dev_buffer.rx_data = Data;
  Dev_buffer.rx_length = 1;
  
  return I2C_MasterTransferData(LPC_I2C1, &Dev_buffer, I2C_TRANSFER_POLLING);
}


Status I2C_8574_ReadInput (uint8_t address, uint8_t* Data, uint8_t Input_Mask)
{
   I2C_M_SETUP_Type Dev_buffer;
   uint8_t DataBuf;
	
  if (address < 10) {
    Dev_buffer.sl_addr7bit = (i2c_input >> 1) + address ;
	Input_Mask = 0xFF;
   }
  else {
    Dev_buffer.sl_addr7bit = address >> 1 ;
  }
  I2C_8574_ReadOutput (address,&DataBuf);
  DataBuf |= Input_Mask;
  Dev_buffer.tx_data = &DataBuf;
  Dev_buffer.tx_length = 1;
  Dev_buffer.retransmissions_max = 5;
  Dev_buffer.rx_data = Data;
  Dev_buffer.rx_length = 1;
  
  return I2C_MasterTransferData(LPC_I2C1, &Dev_buffer, I2C_TRANSFER_POLLING);
}
