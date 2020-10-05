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
 *        This file contains functions to control the Edip lcd display. Please include  "EDIP_240B.h" to use these functions
 * \author
 *         Mark van der Heijden <m.vanderheijden@custommate.eu>
 * 
 */
#ifndef I2C_EDIP_240B_Display
#define I2C_EDIP_240B_Display

  #include "EDIP_240B_Types.h"
  
  #include <stdint.h>
  #include <stdbool.h>

  ///////////////////Global functions////////////////////////////////
  /**
  *@brief Clear the whole display
  *@return SUCCESS or ERROR
  **/
  Status Edip_Clear_Display(void);
  
  /**
  *@brief Set display off (The Edip can still receive commands)
  *@return SUCCESS or ERROR
  **/
  Status Edip_Display_off(void);

  /**
  *@brief Set display on
  *@return SUCCESS or ERROR
  **/
  Status Edip_Display_on(void);
  
    /**
  *@brief LED backlight n1=0: OFF; n1=1: ON; ts=2..255: switches backlight on for ts /10s and then off
  *@param mode
  *@return SUCCESS or ERROR
  **/
  Status Edip_Backlight_Led(uint8_t mode);
  
  /**
  *@brief Adjust brightness of backlight n1=0..100% (non linear)
  *@param procent
  *@return SUCCESS or ERROR
  **/
  Status Edip_Backlight_Led_Brightness(uint8_t procent);

  /**
   * @brief set timeout
   * @param timeout in 1/100 sec
   */
  Status Edip_Set_Timout(uint8_t timeout);
#endif
 /**
 * 	@}
 * @}
 **/
