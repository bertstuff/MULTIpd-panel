#include "SC16IS740.h"
#include <Core/Debug/Debug_Terminal.h>

//-------prototypes ----------
unsigned int SerAdres(unsigned int Adres);

unsigned int SelectReg(tSerReg Register);
	
Status PreSelectReg(unsigned int Adres, tSerReg Register);

int OldLCR = 0x00;

int OldMCR = 0x00 ;

//-------Functies-------------
 
unsigned int SerAdres(unsigned int Adres)
{ switch(Adres){	//koppelbaar met A1,A0 aan VCC/GND (geen SDA/SCL)
   case 0: Adres = 0x9A; break;
   case 1: Adres = 0x98; break;
   case 2: Adres = 0x92; break;
   case 3: Adres = 0x90; break;
   default: Adres = Adres; break; //overige adressen (0x90-0xAE) gewoon doorvoeren
  }
  return Adres;
}

unsigned int SelectReg(tSerReg Register)
{unsigned int TempReg;
  switch(Register){
  //General Registers (als LCR[7]=0)
   case SerREG_RHR:   TempReg = 0x00;break;// R  Receive 
   case SerREG_THR:    	TempReg = 0x00;break;// W  Transmit
   case SerREG_IER:    	TempReg = 0x01;break;// RW
   case SerREG_FCR:    	TempReg = 0x02;break;// W
   case SerREG_IIR:   TempReg = 0x02;break;// R
   case SerREG_LCR:    	TempReg = 0x03; break; // RW
   case SerREG_MCR:    	TempReg = 0x04;break;// RW
   case SerREG_LSR:   TempReg = 0x05;break;// R
   case SerREG_MSR:   TempReg = 0x06;break;// R
   case SerREG_SPR:    	TempReg = 0x07;break;// RW
   case SerREG_TCR:    	TempReg = 0x06;break;// RW (als MCR[2]=1 en EFR[4]=1)
   case SerREG_TLR:    	TempReg = 0x07;break;// RW (als MCR[2]=1 en EFR[4]=1)
   case SerREG_TXLVL:    TempReg = 0x08;break;// R
   case SerREG_RXLVL:    TempReg = 0x09;break;// R
   case SerREG_IOControl:  TempReg = 0x0E;break;// RW
  //Special Registers (als LCR[7]=1 en LCD!=0xBF)
   case SerREG_EFCR:    TempReg = 0x0F;break;// RW
   case SerREG_DLL:    	TempReg = 0x00;break;// RW
   case SerREG_DLH:    	TempReg = 0x01;break;// RW
  //Enhanced Registers (als LCR=0xBF)
   case SerREG_EFR:    	TempReg = 0x02;break;// RW
   case SerREG_XON1:    TempReg = 0x04;break;// RW
   case SerREG_XON2:    TempReg = 0x05;break;// RW
   case SerREG_XOFF1:  	TempReg = 0x06;break;// RW
   case SerREG_XOFF2:  	TempReg = 0x07;break;// RW
   default : TempReg = 0xFF;
  }
  return TempReg ;
}
///////////////////////////////////////////////////////////////////				
Status PreSelectReg(unsigned int Adres, tSerReg Register)
{
 unsigned char DataBuf[10] ;
 bool send = false;
 I2C_M_SETUP_Type Dev_buffer;
 Adres = SerAdres(Adres);

  if ((Register>SerREG_General) & (Register<SerREG_Specials)){ //Gewoon Register
    if ((OldLCR & 0x80)!= 0x00){ //LCR bit 7 is actief, clearen!
      DataBuf[0]= 0x03<<3;	//SubAdres 0x03 kieze= LCR
      DataBuf[1]= OldLCR & ~0x80; //LCR bit 7 clearen (gewone registers actief)
      OldLCR = OldLCR & ~0x80;	//LCR verwijst naar gewone registers
      send  = true;
    }
   }
  else if ((Register>SerREG_Specials) & (Register<SerREG_Enhanced)){ //Special Register
    if ((OldLCR & 0x80)!= 0x80) { //LCR bit 7 is clear, Setten!
      DataBuf[0]= 0x03<<3;	//SubAdres 0x03 kieze= LCR
      DataBuf[1]= OldLCR | 0x80;	//LCR bit 7 setten (Special registers actief)
      OldLCR = OldLCR | 0x80;	//LCR verwijst naar Special registers
      send  = true;
    }
    if (OldLCR == 0xBF) { //LCR =0xBF -> 0x80 maken
      DataBuf[0]= 0x03<<3;	//SubAdres 0x03 kieze= LCR
      DataBuf[1]= 0x80;	//LCR =80 (Special registers actief)
      OldLCR =0x80;	//LCR verwijst naar Special registers
      send  = true;
    }
   }
  else if (Register>SerREG_Enhanced){ //Enhanced Register
    if (OldLCR !=0xBF){ //LCR bit 7 is actief, clearen!
      DataBuf[0]= 0x03<<3;	//SubAdres 0x03 kieze= LCR
      DataBuf[1]= 0xBF;	//LCR waarde = 0xBF (Enhanced registers actief)
      OldLCR =0xBF;	//LCR Enhanced naar gewone registers
      send  = true;
    }
  }
  
  if( send  == true)
  {
    Dev_buffer.sl_addr7bit = Adres>>1;
    Dev_buffer.tx_data = DataBuf;
    Dev_buffer.tx_length = 2;
    Dev_buffer.retransmissions_max = 5;
    Dev_buffer.rx_data = NULL;
    Dev_buffer.rx_length = 0;
    return I2C_MasterTransferData(LPC_I2C1, &Dev_buffer, I2C_TRANSFER_POLLING);
  }
  return SUCCESS;
}   
///////////////////////////////////////////////////////////////////
Status SerInitUART(unsigned int Adres, Ser_UART_Config_t Config)
{
  uint16_t Div;
  Status TransOke = SUCCESS;
  uint8_t RegVal;
  Adres=SerAdres(Adres);
  
//Divisor = (Xtal/Prescale)/(Baudrate*16)
//Xtal = 11.059.200 Hz, Prescale=1
//Divisor = 691200/Baudrate =
  Div = 691200/Config.Baudrate;
  TransOke &= SerWriteReg(Adres,SerREG_DLL,(unsigned char) Div);//Divisor Register Low Byte
  TransOke &= SerWriteReg(Adres,SerREG_DLH,(unsigned char)(Div >> 8));//Divisor Register High Byte
  
  
  if(Config.RS485){
    RegVal = (1<<4); //enable rs485 flow control
    RegVal |= (1<<5); //invert flow control
    SerWriteReg(Adres,SerREG_EFCR,RegVal);
  } 

  RegVal = 0x10;
  RegVal |= Config.AutoCTS<<7;	//CTS Auto flow control, only sent when other sides allows by pulling CTS low.
  RegVal |= Config.AutoRTS<<6;	//RTS Auto flow control, Only allow data to be received when when there is room in the Fifo.
  TransOke &= SerWriteReg(Adres,SerREG_EFR,RegVal);//Enable Enhanced Functions
  TransOke &= SerWriteReg(Adres,SerREG_MCR,(0x00|(Config.RTSActive<<1)));//Clock prescale 1/1 + RTS 
  
  
  RegVal = Config.NumBits - 5;
  if (RegVal > 3){
    RegVal = 0x03;
    printf("SC16IS740: Wrong number of bits \r\n");
  }
  RegVal |= (Config.StopBit > 1 ? 1:0) << 2;
  RegVal |= (Config.Parity & 0x07) << 3;//Parity mode
  TransOke &= SerWriteReg(Adres,SerREG_LCR,RegVal);//juiste aantal bits, stop bits, parity
  TransOke &= SerWriteReg(Adres,SerREG_FCR,0x06);//Reset TxFIFO en RxFIFO
  TransOke &= SerWriteReg(Adres,SerREG_FCR,0x01);//Enable FIFO
  return TransOke ;
}

Status SerWriteReg(unsigned int Adres, tSerReg Register, unsigned char RegValue)
{
  unsigned char DataBuf[10];  
  I2C_M_SETUP_Type Dev_buffer;
  
  Adres = SerAdres(Adres);
  if(PreSelectReg(Adres,Register) != SUCCESS)
  {  
	return ERROR;
  }
  
  DataBuf[0]= SelectReg(Register)<<3;	//Register adres
  if (Register==SerREG_LCR)
  { 
	OldLCR = RegValue;
  }
  
  if (Register==SerREG_MCR)
  { 
	OldMCR = RegValue;
  }  
  
  DataBuf[1]= RegValue;	//Register waarde 
  
  Dev_buffer.sl_addr7bit = Adres>>1;
  Dev_buffer.tx_data = DataBuf;
  Dev_buffer.tx_length = 2;
  Dev_buffer.retransmissions_max = 5;
  Dev_buffer.rx_data = NULL;
  Dev_buffer.rx_length = 0;
  
  return I2C_MasterTransferData(LPC_I2C1, &Dev_buffer, I2C_TRANSFER_POLLING);
} 
///////////////////////////////////////////////////////////////////				
Status SerReadReg(unsigned int Adres, tSerReg Register, unsigned char *pRegValue)
{
  unsigned char TxReg;
  I2C_M_SETUP_Type Dev_buffer;

  Adres = SerAdres(Adres);
  if(PreSelectReg(Adres,Register) != SUCCESS)
  {  
	return ERROR;
  }
  
  TxReg = SelectReg(Register)<<3;	//Register adres
  
  Dev_buffer.sl_addr7bit = Adres>>1;
  Dev_buffer.tx_data = &TxReg;
  Dev_buffer.tx_length = 1;
  Dev_buffer.retransmissions_max = 5;
  Dev_buffer.rx_data = pRegValue;
  Dev_buffer.rx_length = 1;
  
  return I2C_MasterTransferData(LPC_I2C1, &Dev_buffer, I2C_TRANSFER_POLLING);
} 

///////////////////////////////////////////////////////////////////				
Status SerOut(unsigned int Adres, char *pTextString, unsigned int TxLen)
{//verzending kost ongeveer 2,1msec per byte (bij 4800bps)+5msec
  I2C_M_SETUP_Type Dev_buffer;		
  char DataBuf[64] ;
  int i;
 
  if(PreSelectReg(Adres,SerREG_THR) != SUCCESS)
  {  
	return ERROR;
  }
 
  Adres = SerAdres(Adres);

  DataBuf[0]= SelectReg(SerREG_THR)<<3;	//Register adres
  for(i=0;i<TxLen;i++){
    DataBuf[i+1] = pTextString[i];
  }
  //memcpy(DataBuf+1,pTextString,TxLen);
  
  Dev_buffer.sl_addr7bit = Adres>>1;
  Dev_buffer.tx_data = (uint8_t *)DataBuf;
  Dev_buffer.tx_length = TxLen+1;
  Dev_buffer.retransmissions_max = 5;
  Dev_buffer.rx_data = NULL;
  Dev_buffer.rx_length = 0;
  
  return I2C_MasterTransferData(LPC_I2C1, &Dev_buffer, I2C_TRANSFER_POLLING);
} 
///////////////////////////////////////////////////////////////////				
Status SerIn(unsigned int Adres,char *pDataBuf, unsigned int RxLen)
{//verzending kost ongeveer 2,1msec per byte (bij 4800bps)+5msec
  I2C_M_SETUP_Type Dev_buffer;			
  unsigned int TxReg[1];

  if(PreSelectReg(Adres,SerREG_THR) != SUCCESS)
  {  
	return ERROR;
  }
  
  Adres = SerAdres(Adres);
  TxReg[0] = SelectReg(SerREG_RHR)<<3;	//Register adres

  Dev_buffer.sl_addr7bit = Adres>>1;
  Dev_buffer.tx_data = (uint8_t *)TxReg;
  Dev_buffer.tx_length = 1;
  Dev_buffer.retransmissions_max = 5;
  Dev_buffer.rx_data = (uint8_t*)pDataBuf;
  Dev_buffer.rx_length = RxLen;
  
  return I2C_MasterTransferData(LPC_I2C1, &Dev_buffer, I2C_TRANSFER_POLLING);
}

///////////////////////////////////////////////////////////	
Status SerSetDTR(unsigned int Adres,bool stat){
  return SerWriteReg(Adres,SerREG_MCR, stat == true ? OldMCR|(1<<0):OldMCR&(~(1<<0)));
}

Status SerSetRTS(unsigned int Adres,bool stat){
  return SerWriteReg(Adres,SerREG_MCR, stat == true ? OldMCR|(1<<1):OldMCR&(~(1<<1)));
}
