/**
 * \addtogroup Debug
 * @{
 */

/**
 * \defgroup Terminal
 *
 * API to control output debug information to the terminal
 *
 * @{
 */

/**
 * \file
 *        This file contains functions to output debug information to the terminal
 * \author
 *         Mark van der Heijden <m.vanderheijden@custommate.eu>
 * 
 */
#ifndef DEBUG_TERMINAL
#define DEBUG_TERMINAL
  #include <stdio.h>
  #include <stdint.h>
  #include <stdarg.h>
  #include <Board/Def_config.h>

	#if (SERIAL_TERMINAL_ENABLE == 1)
		#include "Serial_Terminal.h"
	#endif
	#if (TCP_TERMINAL_ENABLE == 1)
		#include "TCP_Terminal.h"
	#endif

  /**
  *@brief log and print a error
  *@param format data to log
  *@return void
  **/
  int log_error( const char * format, ... );

#endif
 /**
 * 	@}
 * @}
 **/
