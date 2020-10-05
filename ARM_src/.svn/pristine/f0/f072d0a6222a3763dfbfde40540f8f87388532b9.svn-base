 /**
 * \addtogroup Peripherals
 * @{
 */

/**
 * \defgroup I2C_Dev_Init
 *
 * UART init for devices
 *
 * @{
 */

/**
 * \file
 *       This file contains functions to initialize the UART bus
 * \author
 *         Mark van der Heijden <m.vanderheijden@custommate.eu>
 * 
 */
 #ifndef UART_DEV_INIT
#define UART_DEV_INIT
//////////////////Includes///////////////////
#include <stdint.h>  
#include <stdbool.h>
#include <Core/Process/process.h>
#include <Driver/include/lpc17xx_uart.h>
#include <Driver/include/lpc17xx_pinsel.h>
#include <Driver/include/lpc17xx_nvic.h>
#include <Driver/include/lpc_types.h>
#include <Core/protocol/FAT/ff.h>
/////////////////Typedef////////////////////////

typedef enum{
	UART1_P0 = 0, //uart1 bus on pin 15/16 port 0
	UART1_P2 = 1, //uart1 bus on pin 0/1 port 2
	Basis_EBM_COM2 = 1,//connector X8
	UART2_P2 = 2, //uart2 bus on pin 8/9 port 2
	Basis_EBM_COM1 = 2,//connector X6
	UART3_P4 = 3,  //uart3 bus on pin 28/29 port 4
	Basis_EBM_COM3 = 3,//connector X4

	I2C_UART0 = 4, //SC16IS740 addr0
	I2C_UART1 = 5, //SC16IS740 addr1
	I2C_UART2 = 6, //SC16IS740 addr2
	I2C_UART3 = 7, //SC16IS740 addr3
}UART_select_t;

typedef struct { 
   void (* New_data)(char);                  //New data function
   UART_CFG_Type settings;
   bool Auto_RTS;
   bool Auto_CTS;
} UART_config_type;
//////////////////Global variables////////////////

/////////////////Global functions///////////////

  /**
  * @brief init uart poort
  * @param Uart select uart
  * @param Config configuration struct
  */
 Status UART_poort_Init(UART_select_t Uart,UART_config_type Config);

  /*
   * @brief uart send
   * @param Uart select uart
   * @param data to send
   * @param lenght of data
   * @return data send lenght
   */
  uint32_t UART_send(UART_select_t Uart, uint8_t *data, uint8_t lenght);

  /*
   * @brief uart send
   * @param Uart select uart
   * @param data buffer pointer
   * @param lenght of data to read
   * @return data receive lenght
   */
  int32_t UART_receive(UART_select_t Uart, uint8_t *data,uint8_t  lenght);

  /**
  *@brief Init the uart1 bus on pin 15/16 port 0
  *@param Config configuration struct 
  **/
  void UART1_P0_Dev_Init(UART_config_type Config);
  
  /**
  *@brief Init the uart1 bus on pin 0/1 port 2
  *@param Config configuration struct 
  **/
  void UART1_P2_Dev_Init(UART_config_type Config);
    
  /**
  *@brief Init the uart2 bus on pin 8/9 port 2
  *@param Config configuration struct 
  **/
  void UART2_P2_Dev_Init(UART_config_type Config);

  /**
  *@brief Init the uart3 bus on pin 28/29 port 4
  *@param Config configuration struct 
  **/  
  void UART3_P4_Dev_Init(UART_config_type Config);

  /**
  *@brief Init the uart1 as rs485 bus on pin 15/16 port 0 
  *@param Config configuration struct 
  *@param address module address.
  **/  
  void UART1_RS485_P0_Dev_Init(UART_config_type Config, uint8_t address);

  /**
   * @brief get uart number/name from ini file
   * @param Section section of ini file
   * @param Key of ini file
   * @param DefValue Default string in the event of a failed read
   * @param Description when not NULL and Key not found save defval and description
   * @param Filename the name and full path of the .ini file to read from
   */
  UART_select_t ini_getuart(const TCHAR *Section, const TCHAR *Key, const TCHAR *DefValue, const TCHAR *Description, const TCHAR *Filename);

#endif
 /**
 * 	@}
 * @}
 **/
