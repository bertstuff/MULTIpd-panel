/**
 * \addtogroup device
 * @{
 */

/**
 * \defgroup SC16IS740_API
 *
 * The SC16IS740 API defines a set of functions for controlling a uart over I2C
 *
 * @{
 */

/**
 * \file
 * This file contains functions to write data to the i2c to uart converter.
 * \author
 *         Mark van der Heijden <m.vanderheijden@custommate.eu>
 * 
 */
  #ifndef _SC16IS740_
  #define _SC16IS740_ 
  
  #include <string.h>
  #include <stdlib.h> 
  #include <stdbool.h>
  #include <Driver/include/lpc17xx_i2c.h>
    
  #define I2C_BUSSPEED 400000 ;
  
	typedef enum 	tSerReg {	
	   SerREG_Nul = -1,		//Onbekend
	  //General Registers (als LCR[7]=0)
	   SerREG_General = 0,	//Startgrens van Gewone registers
		SerREG_RHR,			//0x00 R  Receive 
		SerREG_THR,			//0x00 W  Transmit
		SerREG_IER,			//0x01 RW
		SerREG_FCR,			//0x02 W
		SerREG_IIR,			//0x02 R
		SerREG_LCR,			//0x03 RW
		SerREG_MCR,			//0x04 RW
		SerREG_LSR,			//0x05 R
		SerREG_MSR,			//0x06 R
		SerREG_SPR,			//0x07 RW
		SerREG_TCR,			//0x06 RW (als MCR[2]=1 en EFR[4]=1)
		SerREG_TLR,			//0x07 RW (als MCR[2]=1 en EFR[4]=1)
		SerREG_TXLVL,		//0x08 R
		SerREG_RXLVL,		//0x09 R
		SerREG_IOControl,	//0x0E RW
		SerREG_EFCR,		//0x0F RW
	  //Special Registers (als LCR[7]=1 en LCD!=0xBF)
	   SerREG_Specials,		//Startgrens van Speciale registers
		SerREG_DLL,			//0x00 RW
		SerREG_DLH,			//0x01 RW
	  //Enhanced Registers (als LCR=0xBF)
	   SerREG_Enhanced, 	//Startgrens van Enhanced registers
		SerREG_EFR,			//0x02
		SerREG_XON1,		//0x04
		SerREG_XON2,		//0x05
		SerREG_XOFF1,		//0x06
		SerREG_XOFF2,		//0x07
	}tSerReg;	
  
  typedef enum tParity{
     PAR_None	= 0x00,
     PAR_Odd	= 0x01,
     PAR_Even	= 0x02,
     PAR_Mark	= 0x05, 
     PAR_Space	= 0x07 
  } Parity_t;  

  typedef struct{
    uint32_t Baudrate;
    uint8_t NumBits;
    Parity_t Parity;
    uint8_t StopBit;
    bool AutoCTS;
    bool AutoRTS;
    bool RTSActive;
    bool RS485;
  }Ser_UART_Config_t; 
        
        /**
	*@brief Init the serial
	*@param Adres I2C Address of the SC16IS740
	*@param Config Config structure
	*@return SUCCESS or ERROR
	**/         
  Status SerInitUART(unsigned int Adres, Ser_UART_Config_t Config);
           
        /**
	*@brief write data to the configuration registers
	*@param Adres I2C Address of the SC16IS740
	*@param Register Register to write the setting
	*@param RegValue The Value to write to the selected register 
	*@return SUCCESS or ERROR
	**/ 
	Status SerWriteReg(unsigned int Adres, tSerReg Register, unsigned char RegValue);

        /**
	*@brief Read data from configuration register
	*@param Adres I2C Address of the SC16IS740
	*@param Register Register to read from
	*@param RegValue pointer to receive buffer 
	*@return SUCCESS or ERROR
	**/ 	
	Status SerReadReg(unsigned int Adres, tSerReg Register, unsigned char *pRegValue);
	
	/**
	*@brief Send data to UART
	*@param Adres I2C Address of the SC16IS740
	*@param pTextString Data to send to the uart 
	*@param TxLen The number of bits to send 
	*@return SUCCESS or ERROR
	**/ 		
	Status SerOut(unsigned int Adres, char *pTextString, unsigned int TxLen);

	/**
	*@brief Read data from UART
	*@param Adres I2C Address of the SC16IS740
	*@param pDataBuf pointer to receive buffer 
	*@param TxLen The number of bits to read
	*@return SUCCESS or ERROR
	**/ 			
	Status SerIn(unsigned int Adres, char *pDataBuf, unsigned int RxLen);

        /**
	*@brief Set DTR On or Off
	*@param Adres I2C Address of the SC16IS740
	*@param stat true or false
	*@return SUCCESS or ERROR
	**/ 
        Status SerSetDTR(unsigned int Adres, bool stat);

        /**
	*@brief Set RTS On or Off
	*@param Adres I2C Address of the SC16IS740
	*@param stat true or false
	*@return SUCCESS or ERROR
	**/ 
        Status SerSetRTS(unsigned int Adres, bool stat);

#endif //SC16IS740
 /**
 * 	@}
 * @}
 **/
