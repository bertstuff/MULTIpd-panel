 /**
 * \addtogroup Peripherals
 * @{
 */

/**
 * \defgroup I2C_Dev_Init
 *
 * I2C init for devices to check max speed
 *
 * @{
 */

/**
 * \file
 *       This file contains functions to initialize the I2C bus
 * \author
 *         Mark van der Heijden <m.vanderheijden@custommate.eu>
 * 
 */
 #ifndef I2C_DEV_INIT
#define I2C_DEV_INIT
//////////////////Includes///////////////////
#include <stdint.h>  
//////////////////Global variables////////////////
 extern uint32_t I2C_Cur_Speed;

/////////////////Global functions///////////////
  /**
  *@brief Init the I2C bus
  *@param Max_speed_Hz maximum speed of device
  **/
 void I2C_Dev_Init(uint32_t Max_speed_Hz);

#endif
 /**
 * 	@}
 * @}
 **/