#include "EDIP_240B_Send.h"
#include "EDIP_240B_Init.h"
#include "EDIP_240B_Touch.h"
#include <Core/Peripherals/I2C_Dev_Init.h>
#include <Driver/include/lpc17xx_i2c.h>
#include <Core/Timer/timer.h>


uint8_t Edip_address = 0;
bool Edip_Refresh;
Status Edip_Receive(void);
struct timer Edip_delay;

///////////////Prototype/////////////////////////////////////////////
Status Edip_Send(char * txbuffer, uint8_t lenght, bool setting);
char CalcCheckSum(char * data, uint8_t Count);

///////////////Global functions//////////////////////////////////////

Status Edip_Receive(void){
  I2C_M_SETUP_Type Dev_buffer;    
  uint8_t txbuffer[5];
  uint8_t rxbuffer[10];
  uint8_t data_len;
  uint8_t rx_checksum;
  uint8_t calc_checksum;
 
  //Send request
  txbuffer[0]= DC2LCD;
  txbuffer[1]= 1;
  txbuffer[2]= 'S';
  txbuffer[3]= (uint8_t)CalcCheckSum((char *)txbuffer, 3);
  
  Dev_buffer.sl_addr7bit = Edip_address >> 1;
  Dev_buffer.retransmissions_max = 5;
  Dev_buffer.tx_data = txbuffer;
  Dev_buffer.rx_data = rxbuffer;
  Dev_buffer.tx_length = 4;
  Dev_buffer.rx_length = 1;
  I2C_MasterTransferData(LPC_I2C1, &Dev_buffer, I2C_TRANSFER_POLLING); 


  //Receive answer
  if(rxbuffer[0] != ACKLCD){return ERROR;}//Receive acknowledge
  Dev_buffer.tx_length = 0;
  Dev_buffer.rx_length = 1;
  I2C_MasterTransferData(LPC_I2C1, &Dev_buffer, I2C_TRANSFER_POLLING);//Receive begin of message
  if(rxbuffer[0] != DC1LCD){return SUCCESS;}//If byte is not begin of message escape
  I2C_MasterTransferData(LPC_I2C1, &Dev_buffer, I2C_TRANSFER_POLLING);//Receive length of buffer 
  if(rxbuffer[0] == 0){// buffer empty
    I2C_MasterTransferData(LPC_I2C1, &Dev_buffer, I2C_TRANSFER_POLLING);//Receive checksum byte
    return SUCCESS;
  }
  data_len = rxbuffer[0] + 1;
  if(data_len > 10){return ERROR;}
  Dev_buffer.rx_length = data_len;
  I2C_MasterTransferData(LPC_I2C1, &Dev_buffer, I2C_TRANSFER_POLLING);//Receive all bytes + checksum
  rx_checksum = rxbuffer[data_len - 1];//get received checksum
  calc_checksum = DC1LCD + (data_len - 1) + CalcCheckSum((char*)rxbuffer, data_len - 1);//Calc checksum
  if(rx_checksum != calc_checksum){return ERROR;};//compare checksum  
  
  //Parse Received packet
  switch(rxbuffer[1]){
  case 'A'://Key response
      Exec_Touch_handler(rxbuffer[3]);
    break;
  default:
    break;
  }
  return SUCCESS;
}

Status Edip_Send(char * txbuffer, uint8_t lenght, bool setting){
  I2C_M_SETUP_Type Dev_buffer;
  Status I2C_Status = ERROR;
  char rxbuffer[1];
  uint8_t retrying = 0;
  char buffer[100];
  uint8_t len;
  uint8_t packet;
  
  while(Edip_Init == false){process_run();};//Wait for init to complete 

  for(packet = 0;packet <= (lenght/61);packet++){


	  while(1){
		  retrying++;
		 if(retrying > 4){
			 break;
		 }
		  len = ((lenght-(packet*61)) > 61)? 61 : (lenght-(packet*61));
		  if(setting == false){
			  buffer[0] = DC1LCD;
		  }else{
			  buffer[0] = DC2LCD;
		  }
		  buffer[1] = len;
		  memmove(&buffer[2],&txbuffer[(packet*61)],len);
		  buffer[len+2] = CalcCheckSum(buffer,len+2);

		  I2C_SetClock (LPC_I2C1, 50000);
		  Dev_buffer.sl_addr7bit = Edip_address >> 1;
		  Dev_buffer.tx_data = (uint8_t *)buffer;
		  Dev_buffer.tx_length = len+3;
		  Dev_buffer.retransmissions_max = 5;
		  Dev_buffer.rx_data = NULL;
		  Dev_buffer.rx_length = 0;


		 I2C_Status = I2C_MasterTransferData(LPC_I2C1, &Dev_buffer, I2C_TRANSFER_POLLING);
		 if(I2C_Status == ERROR){
			printf("Write to edip failed\r\n");
			timer_set(&Edip_delay,CLOCK_SECOND*2);
			while(!timer_expired(&Edip_delay)){
				 ;
			}
			continue;
		 }

		 timer_set(&Edip_delay,(lenght/10)+1);
		 while(!timer_expired(&Edip_delay)){
			 ;
		 }

		  Dev_buffer.sl_addr7bit = Edip_address >> 1;
		  Dev_buffer.tx_data = NULL;
		  Dev_buffer.tx_length = 0;
		  Dev_buffer.retransmissions_max = 5;
		  Dev_buffer.rx_data = (uint8_t *)rxbuffer;
		  Dev_buffer.rx_length = 1;
		 I2C_Status = I2C_MasterTransferData(LPC_I2C1, &Dev_buffer, I2C_TRANSFER_POLLING);
		 if(I2C_Status == ERROR){
			printf("read edip failed\r\n");
			printf("Write to edip failed\r\n");
			timer_set(&Edip_delay,CLOCK_SECOND*2);
			while(!timer_expired(&Edip_delay)){
				 ;
			}
			continue;
		 }

		 if(rxbuffer[0] == 0x06){
			 break;
		 }

		 if(rxbuffer[0] != 0x15){
			 timer_set(&Edip_delay,CLOCK_SECOND*2);
			 while(!timer_expired(&Edip_delay)){
				 ;
			 }
		 }

		 printf("Edip retry %02x\r\n",rxbuffer[0]);
	 }
  }
  I2C_SetClock (LPC_I2C1, I2C_Cur_Speed);
  return I2C_Status;
}

///////////////local functions//////////////////////////////////////
char CalcCheckSum(char * data, uint8_t Count) //Bereken checksum of xx bytes van DataBuf
{ 
  char TempByte = 0;
  uint8_t i;

  for (i=0;i<Count;i++){
    TempByte += data[i];
  }
  return TempByte;
}

