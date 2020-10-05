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
 *       This file contains functions to initialize the Edip lcd screen. Please include  "EDIP_240B.h" to use these functions
 * \author
 *         Mark van der Heijden <m.vanderheijden@custommate.eu>
 * 
 */
#ifndef I2C_EDIP_240B_INIT
#define I2C_EDIP_240B_INIT

  #include "EDIP_240B_Types.h"
  
  #include <stdint.h>
  #include <stdbool.h>
  #include <Core/Timer/etimer.h>
  #include <Driver/include/lpc17xx_i2c.h>

  #define EDIP_MAX_SPEED_HZ 100000
  ///////////////////Global variables//////////////////////////////// 
  extern bool Edip_Init; 
 
  ///////////////////Global functions////////////////////////////////
  PROCESS_NAME(EDIP_process);
	
  /**
  *@brief initialize EDIP screen (Please call process "EDIP_process" to execute this function)
  **/  
  void Edip_init_LCD(void);
  
  /**
  *@brief Select current EDIP address
  *@param Address Address of edip (0..9 or 0x00 ...0xFF)
  **/  
  void Edip_Select_LCD(uint8_t Address);

#endif
 /**
 * 	@}
 * @}
 **/
