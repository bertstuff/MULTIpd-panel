/**
 * \addtogroup device
 * @{
 */

/**
 * \defgroup 8574_API
 *
 * The 8574 API defines a set of functions for accessing the io expander .
 *
 * @{
 */

/**
 * \file
 *        API file for the 8574 io expander. The functions can be used in two different ways. if you define address between 0 - 7  then 
 *		  the functions using the Navmate rules to use the LPC8574 as input and the LPC8574A as output
 *
 *		  You can choose to use a port as in or output with the Input_Mask 
 *		  For the PCF8574 use address 0x40 - 0x4E 
 *		  For the PCF8574A use address 0x70 - 0x7E
 * \author
 *         Mark van der Heijden <m.vanderheijden@custommate.eu>
 * 
 */
#ifndef NAVMATE_I2C_8574
#define NAVMATE_I2C_8574

#include <Driver/include/lpc17xx_i2c.h>

#define PCF8574_MAX_SPEED_HZ 400000
#define  PCF8574_addr	0x40 // PCF8574
#define  PCF8574A_addr	0x70 // PCF8574A

/**
*@brief Set max speed for PCF8574
*/
void I2C_8574_Init (void);

/**
*@brief enable/disable output on PCF8574 device 
*@param address I2c address of device (Chan 0-7 or address 0x01-0xFF)
*@param Data pointer to output data
*@param Input_Mask mask byte for data direction  1 = Input 0 = Output 
*@return SUCCESS or ERROR
**/
Status I2C_8574_WriteOutput (uint8_t address, uint8_t* Data, uint8_t Input_Mask);
/**
*@brief Read current output state
*@param address I2c address of device (Chan 0-7 or address 0x01-0xFF)
*@param Data pointer to output data
*@return SUCCESS or ERROR
**/
Status I2C_8574_ReadOutput (uint8_t address, uint8_t* Data);

/**
*@brief write output register to high and read current output state
*@param address I2c address of device (Chan 0-7 or address 0x01-0xFF)
*@param Data pointer to output data
*@param Input_Mask mask byte for data direction  1 = Input 0 = Output 
*@return SUCCESS or ERROR
**/
Status I2C_8574_ReadInput (uint8_t address, uint8_t* Data, uint8_t Input_Mask);

#endif
 /**
 * 	@}
 * @}
 **/

