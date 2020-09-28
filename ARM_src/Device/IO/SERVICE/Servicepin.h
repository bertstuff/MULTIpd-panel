/**
 * \addtogroup device
 * @{
 */

/**
 * \defgroup Servicepin
 *
 * API to check the service pin on the board
 *
 * @{
 */

/**
 * \file
 *        This file contains functions to check the service pin
 * \author
 *         Mark van der Heijden <m.vanderheijden@custommate.eu>
 *
 */
#ifndef SERVICEPIN
#define SERVICEPIN

  /* Include files */
  #include <driver/include/lpc17xx_gpio.h>
  #include <stdbool.h>
  #include <Core/Thread/pt.h>
  #include <Core/Process/process.h>
  #include "config.h"

  /* Global Types */
  #define DIR_INPUT 0
#ifndef BOARD_SERVICE_PORT
  #define BOARD_SERVICE_PORT 2
#endif
#ifndef BOARD_SERVICE_PIN
  #define BOARD_SERVICE_PIN (1<<13)
#endif

  /* Global prototypes */
extern struct pt pin_pt;
/*
 * @brief get status of service pin
 * @return status
 */
bool Servicepin(void);

/*
 * @set pin as output and control led
 */
void set_Servicepin_led(bool state);

/*
 * @brief standaard handler for checking service button(sw1 button1) and service pass
 * @param exit when exit returns true the current screen must be closed
 * @param ev event
 * @param data event data
 */
PT_THREAD(Servicebutton_handler(bool * exit,process_event_t ev, process_data_t data));

#endif
 /**
 * 	@}
 * @}
 **/
