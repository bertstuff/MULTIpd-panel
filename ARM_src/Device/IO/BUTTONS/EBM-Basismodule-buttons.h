
/**
 * \addtogroup device
 * @{
 */

/**
 * \defgroup EBM_button
 *
 * Button functions for the EBM basismodule V1.0
 *
 * @{
 */

/**
 * \file
 *        EBM_button_process reads the status of the buttons and create a event when button status is changed
 * \author
 *         Mark van der Heijden <m.vanderheijden@custommate.eu>
 *
 */
#ifndef EBM_BUTTON_H_
#define EBM_BUTTON_H_

#include <stdbool.h>
#include <stdint.h>
#include <Core/Process/process.h>

  #define DIR_OUTPUT 1
  #define DIR_INPUT 0

  #define EBM_BUTTON1 0
  #define EBM_BUTTON2 1
  #define EBM_BUTTON3 2
  #define EBM_BUTTON4 3
  #define EBM_BUTTON5 4
  #define EBM_BUTTON6 5
  #define EBM_BUTTON7 6
  #define EBM_BUTTON8 7

  typedef struct{
    uint8_t Port_sw;
    uint8_t Pin_sw;
    uint8_t Port_led;
    uint8_t Pin_led;
  }EBM_Button_place_t;

  typedef struct{
    bool Sw_stat;
    bool SW_changed;
    bool Led_stat;
  }EBM_Button_state_t;

  extern EBM_Button_state_t Button_state_list[8];
  extern process_event_t event_EBM_button_Pressed;
  extern process_event_t event_EBM_button_Released;

  PROCESS_NAME(EBM_button_process);

  /*
   * @brief starts button process
   * @param buttons number of buttons
   */
  void EBM_button_init(uint8_t buttons, bool invert);

  /**
   * @brief check with button is pressed
   * @param button button nr to check
   * @retrun true when pressed false when released
   */
  bool EBM_Button_pressed(uint8_t button);

  /**
   * @brief change state of button leds
   * @param Button button 1 t/m 8
   * @param state true = on or false = off
   */
  void EBM_Button_led(int8_t Button, bool state);

#endif
 /**
 * 	@}
 * @}
 **/

