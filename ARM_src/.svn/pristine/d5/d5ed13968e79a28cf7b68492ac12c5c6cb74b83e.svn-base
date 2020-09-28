/**
 * \addtogroup device
 * @{
 */

/**
 * \defgroup i2c_button_API
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
#ifndef _I2C_BUTTON3X4_H_
#define _I2C_BUTTON3X4_H_

#include <Core/Process/process.h>
#include <Driver/include/lpc_types.h>
#include <stdbool.h>
#include <stdint.h>

#ifndef GPIO_ADDR
#define GPIO_ADDR 0xFE
#endif

//Key values
#define C_BUTTON1  1
#define C_BUTTON2  2
#define C_BUTTON3  3
#define C_BUTTON4  4
#define C_BUTTON5  5
#define C_BUTTON6  6
#define C_BUTTON7  7
#define C_BUTTON8  8
#define C_BUTTON9  9
#define C_BUTTON10 "X"
#define C_BUTTON11 0
#define C_BUTTON12 "V"

#define C_RETURN 'X'
#define C_OK 'V'
#define C_IDLE 0xFF

extern uint8_t cur_buttonval;

/**
 * @brief event when key is pressed. Key value added to data
 * @code
 *
 */
extern process_event_t event_Button3x4_pressed;

/**
 *@brief if true = button error
 */
extern Status Button3x4_Error;

/*
 * @brief Init the buttons
 * @param address keypad address 0..7 or address
 * @return void
 */
void Button3x4_I2C_init(uint8_t address);
void Button3x4_IO_init(void);
/*
 * @brief activate or deactivate the backlight and events
 * keypad create event_Button3x4_pressed event when pressed and activated.
 * @param state true = active
 * @return void
 */
void Button3x4_activate(bool state);

/*--------------KEYPAD--------------*/
/*
 * @brief Init the keypad
 * @param address keypad address 0..7 or address IO_ADDR for IO lcd
 * @return void
 */
void keypad_init(uint8_t address);
/*
 * @brief activate or deactivate the backlight and events
 * keypad create event_Button3x4_pressed event when pressed and activated.
 * @param state true = active
 * @return void
 */
#define keypad_activate(STATE)	Button3x4_activate(STATE)

//return true if event is keypad pressed
bool keypad_pressed(process_event_t ev);

//get key data of keypad event
uint8_t keypad_button(process_event_t ev, process_data_t data);


#endif //_I2C_BUTTON3X4_H_
 /**
 * 	@}
 * @}
 **/

