/**
 * \addtogroup device
 * @{
 */

/**
 * \defgroup IO_4Bit_API
 *
 * The io 4Bit API defines a set of functions for the 4x in and 4x relay output .
 *
 * @{
 */

/**
 * \file
 *
 * \author
 *         Mark van der Heijden <m.vanderheijden@custommate.eu>
 *
 */
#ifndef __IO_4BIT_H__
#define __IO_4BIT_H__

#include <stdbool.h>
#include <Device/I2C/8574/I2C_8574.h>

typedef struct{
	uint8_t addr;
	uint8_t data;
	Status error;
}IO_4Bit_t;

/*
 * @brief init 4bit io pcb
 * @param data struct to store current device state
 * @param i2c_addr i2c address to init
 */
void IO_4Bit_init(IO_4Bit_t * data, uint8_t i2c_addr);

/*
 * @brief turn the relay output on
 * @param data struct to store current device state
 * @param output(Bitwise) to turn on (0...0x0F)
 */
Status IO_4Bit_Set_output(IO_4Bit_t * data, uint8_t ouput);

/*
 * @brief turn the relay output off
 * @param data struct to store current device state
 * @param output(Bitwise) to turn off (0...0x0F)
 */
Status IO_4Bit_Clear_output(IO_4Bit_t * data, uint8_t ouput);

/*
 * @brief Get input state
 * @param data struct to store current device state
 * @param input(Bitwise) input to read (0...0x0F)
 */
bool IO_4Bit_Get_input(IO_4Bit_t * data,uint8_t input);
#endif //__IO_4BIT_H__
 /**
 * 	@}
 * @}
 **/
