#ifndef	NAVMATE_I2C_DIGI
#define	NAVMATE_I2C_DIGI
#define  i2c_input	0x40
#define  i2c_output	0x70
#endif

#define Input_Reg 0x00
#define Output_Reg 0x01 
#define Polarity_Reg 0x02
#define Config_Reg 0x03

#include "I2C_9554.h"
#include <Core/Peripherals/I2C_Dev_Init.h>
#include <Core/Debug/Debug_Terminal.h>

/////////////////////////Prototypes//////////////////////////////////
Status I2C_9554_SendData(uint8_t address, uint8_t Register, uint8_t Data);
Status I2C_9554_ReadData(uint8_t address, uint8_t Register, uint8_t * Data);

/////////////////////////Global variable///////////////////////////////////
uint8_t Cur_Command;

/////////////////////////Global functions/////////////////////////////

void I2C_9554_Init (void){
  I2C_Dev_Init(PCF9554_MAX_SPEED_HZ);
}

Status I2C_9554_SetDir (uint8_t address, uint8_t Input_Mask){
  return I2C_9554_SendData(address, Config_Reg, Input_Mask);
}

Status I2C_9554_SetPolarity (uint8_t address, uint8_t Polar_Mask){
  return I2C_9554_SendData(address, Polarity_Reg, Polar_Mask);
}

Status I2C_9554_ReadPort (uint8_t address, uint8_t * Port_Data){
  return I2C_9554_ReadData(address, Input_Reg, Port_Data);
}

Status I2C_9554_WritePort (uint8_t address, uint8_t Port_Data){
  return I2C_9554_SendData(address, Output_Reg, Port_Data);
}

Status I2C_9554_SetPort (uint8_t address, uint8_t Pin_Data){
  uint8_t Port_Stat;
  Status Stat;
  //Stat = I2C_9554_ReadPort (address, &Port_Stat);
  Stat = I2C_9554_ReadData (address, Output_Reg, &Port_Stat);
  if(Stat == ERROR){   
    return ERROR;
  }
  Port_Stat |= Pin_Data;
  return I2C_9554_WritePort (address, Port_Stat);
}

Status I2C_9554_ClearPort (uint8_t address, uint8_t Pin_Data)
{
  uint8_t Port_Stat;
  Status Stat;  
  //I2C_9554_ReadPort (address, &Port_Stat);
  Stat = I2C_9554_ReadData (address, Output_Reg, &Port_Stat);
  if(Stat == ERROR){
    return ERROR;
  }
  Port_Stat &= ~Pin_Data;
  return I2C_9554_WritePort (address, Port_Stat);
}
////////////////////////////////////////local functions/////////////////////////////////
Status I2C_9554_SendData(uint8_t address, uint8_t Register, uint8_t Data){
  I2C_M_SETUP_Type Dev_buffer;
  uint8_t tx[2]; 
  Status Stat;  
  
  tx[0] = Register;
  tx[1] = Data;
  
  if (address < 10){
    Dev_buffer.sl_addr7bit = (i2c_input >> 1) + address ;
   }
  else {
    Dev_buffer.sl_addr7bit = address >> 1 ;
  }  
  Dev_buffer.tx_data = tx;
  Dev_buffer.tx_length = 2;
  Dev_buffer.retransmissions_max = 5;
  Dev_buffer.rx_data = NULL;
  Dev_buffer.rx_length = 0;
  Stat = I2C_MasterTransferData(LPC_I2C1, &Dev_buffer, I2C_TRANSFER_POLLING);
  if(Stat == ERROR){
    printf("PCA9554 addr:0x%02x communication error.\r\n",address);   
  }
  return Stat;
}

Status I2C_9554_ReadData(uint8_t address, uint8_t Register, uint8_t * Data){
  I2C_M_SETUP_Type Dev_buffer;
  Status Stat; 
  
  if (address < 10) {
    Dev_buffer.sl_addr7bit = (i2c_input >> 1) + address ;
   }
  else {
    Dev_buffer.sl_addr7bit = address >> 1 ;
  }
  //Receive Data 
  Dev_buffer.tx_data = &Register;
  Dev_buffer.tx_length = 1;
  Dev_buffer.retransmissions_max = 5;
  Dev_buffer.rx_data = Data;
  Dev_buffer.rx_length = 1;
  Stat = I2C_MasterTransferData(LPC_I2C1, &Dev_buffer, I2C_TRANSFER_POLLING);
  if(Stat == ERROR){
    printf("PCA9554 addr:0x%02x communication error.\r\n",address);   
  }
  return Stat;
}

