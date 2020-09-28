/**
 * \addtogroup device
 * @{
 */

/**
 * \defgroup keypad_API
 *
 * The keypad API has functions to acces the keypad
 *
 * @{
 */

/**
 * \file
 * This file contains functions to control the keypad
 * \author
 *         Mark van der Heijden <m.vanderheijden@custommate.eu>
 *
 */
#ifndef NAVMATE_I2C_KEYPAD
#define NAVMATE_I2C_KEYPAD

#include <Core/Process/process.h>
#include <Driver/include/lpc_types.h>
#include <stdbool.h>
#include <stdint.h>

//Key values
#define C_STER 'X'
#define C_HECK 'V'
#define C_IDLE 0xFF

//Terminal defines
#define T_LEDROOD (1 << 0)
#define T_LEDGEEL (1 << 1)
#define T_LEDGROEN (1 << 2)
#define T_BUZZER (1 << 3)
#define T_OUTPUT1 (1 << 4)
#define T_OUTPUT2 (1 << 5)
#define T_OUTPUT3 (1 << 6)
#define T_OUTPUT4 (1 << 7)
#define T_INPUT1 (1 << 7)


/**
 * @brief event when key is pressed. Key value added to data
 * @code
 *
 */
extern process_event_t event_keypad_pressed;

/**
 *@brief if true = keypad error
 */
extern Status Keypad_Error;

/*
 * @brief Init the keypad
 * @param address keypad address 0..7
 * @return void
 */
void Keypad_init(uint8_t address);

/*
 * @brief activate or deactivate the keypad
 * keypad create event_keypad_presed event when pressed and activated.
 * @param state true = active
 * @return void
 */
void Keypad_activate(bool state);

/*
 * @brief Get value of input terminal
 * @return state of input
 */
bool Keypad_get_input1(void);

/*
 * @brief set output terminal
 * @param Pin pins to select or use Terminal defines
 * @state true or false
 */
void Keypad_set_output(uint8_t Pin, bool state);

#endif
 /**
 * 	@}
 * @}
 **/

