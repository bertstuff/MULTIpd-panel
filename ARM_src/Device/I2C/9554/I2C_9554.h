/**
 * \addtogroup device
 * @{
 */

/**
 * \defgroup 9554_API
 *
 * The 9554 API defines a set of functions for accessing the io expander .
 *
 * @{
 */

/**
 * \file
 *        API file for the 9554 io expander. The functions can be used in two different ways. if you define address between 0 - 7  then 
 *		  the functions using the Navmate rules to use the PCF9554 as input and the PCF9554A as output
 *
 *		  You can choose to use a port as in or output with the Input_Mask 
 *		  For the PCF9554 use address 0x40 - 0x4E 
 *		  For the PCF9554A use address 0x70 - 0x7E
 * \author
 *         Mark van der Heijden <m.vanderheijden@custommate.eu>
 * 
 */
#ifndef I2C_9554
#define I2C_9554

#include <Driver/include/lpc17xx_i2c.h>

#define PCF9554_MAX_SPEED_HZ 400000

/**
*@brief Set max speed for PCF8574
*/
void I2C_9554_Init (void);

/**
*@brief Set data direction of port 
*@param address I2c address of device (Chan 0-7 or address 0x01-0xFF)
*@param Input_Mask mask byte for data direction  1 = Input 0 = Output 
*@return SUCCESS or ERROR
**/
Status I2C_9554_SetDir (uint8_t address, uint8_t Input_Mask);

/**
*@brief Set Polarity of Port. allows the user to invert the polarity of the Input Port data
*@param address I2c address of device (Chan 0-7 or address 0x01-0xFF)
*@param Polar_Mask mask byte for data direction  1 = inverted 0 = normal 
*@return SUCCESS or ERROR
**/
Status I2C_9554_SetPolarity (uint8_t address, uint8_t Polar_Mask);

/**
*@brief reflects the incoming logic levels of the pins, regardless of whether the pin is defined as an input or an output 
*@param address I2c address of device (Chan 0-7 or address 0x01-0xFF)
*@param Port_Data pointer to read buffer byte
*@return SUCCESS or ERROR
**/
Status I2C_9554_ReadPort (uint8_t address, uint8_t * Port_Data);

/**
*@brief  reflects the outgoing logic levels of the pins defined as outputs
*@param address I2c address of device (Chan 0-7 or address 0x01-0xFF)
*@param Port_Data Data to write to port
*@return SUCCESS or ERROR
**/
Status I2C_9554_WritePort (uint8_t address, uint8_t Port_Data);

/**
*@brief Set the outgoing logic levels of the pins defined as outputs
*@param address I2c address of device (Chan 0-7 or address 0x01-0xFF)
*@param Pin_Data Pins to set. a bit written to 1 will be set 
*@return SUCCESS or ERROR
**/
Status I2C_9554_SetPort (uint8_t address, uint8_t Pin_Data);

/**
*@brief Clear the outgoing logic levels of the pins defined as outputs
*@param address I2c address of device (Chan 0-7 or address 0x01-0xFF)
*@param Pin_Data Pins to set. a bit written to 1 will be cleared 
*@return SUCCESS or ERROR
**/
Status I2C_9554_ClearPort (uint8_t address, uint8_t Pin_Data);

#endif
 /**
 * 	@}
 * @}
 **/

