#ifndef	NAVMATE_I2C_Max128
#define	NAVMATE_I2C_Max128
#define  i2c_ADC128	0x50 
#endif

#include "MAX128.h"
#include <Core/Peripherals/I2C_Dev_Init.h>
#include <Core/Timer/timer.h>


static struct timer MAX128_delay;

void I2C_MAX128_Init (void){
	I2C_Dev_Init(MAX128_MAX_SPEED_HZ);
}

Status I2C_MAX128_Read (uint8_t address, uint8_t channel, uint16_t* Data)
{
	 Status stat;
          I2C_M_SETUP_Type Dev_buffer;																//Device Buffer for sending and receiving i2c data
	 uint8_t DataBuf;
	 uint8_t RX_buffer[2];
         
	 DataBuf = 0x88;																			//select start condition (0x80) and set measurement from 0 to Vref (0x08)
	 DataBuf |= ((channel & 0x07)<<4);															//select which channel should be measured (only 0 to 7 possible)
	if (address < 10) {																			//if input address is 0 - 7 convert address to correct value
		Dev_buffer.sl_addr7bit = (i2c_ADC128 >> 1) + address ;		
	}
    else {																						//else use current value as address
		Dev_buffer.sl_addr7bit = address >> 1 ;
	}
	
	 Dev_buffer.tx_data = &DataBuf;
	 Dev_buffer.tx_length = 1;
	 Dev_buffer.retransmissions_max = 5;
	 Dev_buffer.rx_data = NULL;
	 Dev_buffer.rx_length = 0;
	 stat = I2C_MasterTransferData(LPC_I2C1, &Dev_buffer, I2C_TRANSFER_POLLING);
        
         timer_set(&MAX128_delay,1);
         while(!timer_expired(&MAX128_delay)){};
         Dev_buffer.tx_data = NULL;
	 Dev_buffer.tx_length = 0;
	 Dev_buffer.retransmissions_max = 5;
	 Dev_buffer.rx_data = RX_buffer;
	 Dev_buffer.rx_length = 2;
         stat = I2C_MasterTransferData(LPC_I2C1, &Dev_buffer, I2C_TRANSFER_POLLING);         
         *Data =  RX_buffer[1];
         *Data +=  RX_buffer[0] <<8;
         
         
	return stat;
}
