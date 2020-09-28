/**
 * \addtogroup device
 * @{
 */

/**
 * \defgroup MAX128_API
 *
 * The MAX128 API defines a set of functions for accessing the 12-bit ADC.
 *
 * @{
 */

/**
 * \file
 *        API file for the MAX128 12-bit ADC. The functions can be used for reading an analogue signal and convert it to a digital value.
 *		  The address can be selected by inputting a number 0 to 7, or by directly sending its address to the function.
 *		  After selecting chip address the user should select which of the 8 channels it wants to read (0-7).
 *		  The chip will reply with the current value on that channel.
 *
 *		  The address range for the MAX128 is 0x50 defined as I2C_MAX128
 * \author
 *         Steffen Seltonrijch <S.Seltonrijch@custommate.eu>
 * 
 */
#ifndef I2C_MAX128
#define I2C_MAX128

#include <Driver/include/lpc17xx_i2c.h>

#define MAX128_MAX_SPEED_HZ 400000

/**
*@brief Set max speed for PCF8574
*/
void I2C_MAX128_Init (void);

/**
*@brief Read current analogue (RAW) value on the MAX128
*@param address I2c address of device (Chan 0-7 or address 0x01-0xFF)
*@param channel that needs to be read (0-7)
*@param Data pointer to output data
*@return SUCCESS or ERROR
**/
Status I2C_MAX128_Read (uint8_t address, uint8_t channel, uint16_t* Data);
#endif
 /**
 * 	@}
 * @}
 **/

