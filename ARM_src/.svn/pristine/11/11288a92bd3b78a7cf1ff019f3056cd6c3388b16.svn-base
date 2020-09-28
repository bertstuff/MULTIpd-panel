 /**
 * \addtogroup device
 * @{
 */

/**
 * \defgroup EDIP_240B_API
 *
 * EDIP 240B LCD API.
 *
 * @{
 */

/**
 * \file
 *       This file contains functions to send data to the Edip lcd screen.
 * \author
 *         Mark van der Heijden <m.vanderheijden@custommate.eu>
 * 
 */
#ifndef I2C_EDIP_240B_SEND
#define I2C_EDIP_240B_SEND

  #include "EDIP_240B_Types.h"
  #include <Core/Thread/pt.h>
  #include <Core/Process/process.h>
   
  #include <stdint.h>
  #include <stdbool.h>
  #include <string.h>
  #include <stdarg.h>
  #include <stdio.h>

  #define 	ESCLCD	0x1B
  #define 	ACKLCD	0x06
  #define 	DC1LCD	0x11
  #define 	DC2LCD	0x12

  #define       HDR  0x03 //Header bytes
  #define       DST  0x00 //Data Start

  #define INRANGE(i, min, max) (i < min) || (i > max) ? false : true 
   
  extern uint8_t Edip_address;
  extern bool Edip_Refresh;
  
  /**
  *@brief Add header to buffer and send buffer to Edip 
  *@param txbuffer pointer to buffer
  *@param length of data packet
  **/
  Status Edip_Send(char * txbuffer, uint8_t lenght, bool setting);
  
  /**
  *@brief Receive data from Edip buffer
  **/
  Status Edip_Receive(void);
#endif
 /**
 * 	@}
 * @}
 **/
