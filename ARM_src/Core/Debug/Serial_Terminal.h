/**
 * \addtogroup Debug
 * @{
 */

/**
 * \defgroup Terminal
 *
 * API to control output debug information to the Serial terminal
 *
 * @{
 */

/**
 * \file
 *        This file contains functions to output debug information to the Serial terminal
 * \author
 *         Mark van der Heijden <m.vanderheijden@custommate.eu>
 * 
 */
#ifndef SERIAL_DEBUG_TERMINAL
#define SERIAL_DEBUG_TERMINAL
  #include <stdio.h>
  #include <stdint.h>
  #include <stdarg.h>

  #include <Board/Def_config.h>

  extern uint8_t pay_event;

  void Serial_Terminal_init(void);
  
  void Serial_Terminal_send(char * data,int lenght);
  
#endif
 /**
 * 	@}
 * @}
 **/
