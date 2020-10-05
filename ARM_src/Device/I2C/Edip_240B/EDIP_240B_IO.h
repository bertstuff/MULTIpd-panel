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
 *      This file contains functions to control the leds and buzzers
 * \author
 *         Mark van der Heijden <m.vanderheijden@custommate.eu>
 * 
 */
#ifndef I2C_EDIP_240B_IO
#define I2C_EDIP_240B_IO
  
  #include "EDIP_240B_Types.h"
  #include <stdint.h>
  #include <stdbool.h>
  #include <string.h>

  ///////////////////IO functions////////////////////////////////
  /**
  *@brief init io expander
  **/
  void Edip_io_init(void);
  
  /**
  *@brief Switch beeper output (pin 16) ts=2..255 for ts 1/10s to high ts=0 set permanent low, ts=1 set permanent high
  *@param mode
  *@return SUCCESS or ERROR
  **/
  Status Edip_Set_Buzzer(uint8_t mode);
  
  /**
  *@brief Set the status of the Big alarm buzzer
  *@param Status Buzzer on/off
  *@return SUCCESS or ERROR
  **/
  void Edip_Set_Alarm_Buzzer(bool Status);

  /**
  *@brief Set the status of the alarm leds
  *@param Status Led on/off
  *@return SUCCESS or ERROR
  **/
  void Edip_Set_Alarm_Led(bool Status);
  
  /**
  *@brief Set toggle alarm buzzer on interval
  *@param onTime on time * (1/4 sec)
  *@param offTime off time * (1/4 sec)
  *@return void
  **/
  void Edip_Set_Alarm_Buzzer_interval(uint16_t onTime, uint16_t offTime);

  /**
  *@brief Set toggle alarm led on interval
  *@param onTime on time * (1/4 sec)
  *@param offTime off time * (1/4 sec)
  *@return void
  **/
  void Edip_Set_Alarm_Led_interval(uint16_t onTime, uint16_t offTime);
  
  /////////////////private functions///////////////
  /**
  *@brief handler io interval 
  */
  void EDIP_IO_interval_handler(void);
#endif
 /**
 * 	@}
 * @}
 **/
