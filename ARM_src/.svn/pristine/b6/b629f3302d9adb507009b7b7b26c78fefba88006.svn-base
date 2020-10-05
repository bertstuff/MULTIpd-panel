/**
 * \addtogroup Debug
 * @{
 */

/**
 * \defgroup Hardware check
 *
 * API to check the status of the hardware
 *
 * @{
 */

/**
 * \file
 *        This file contains functions to check the status of the hardware
 * \author
 *         Mark van der Heijden <m.vanderheijden@custommate.eu>
 * 
 */
#ifndef __ERROR_CHECK_H__
#define __ERROR_CHECK_H__

#include <Core/Type/type.h>
#include <stdbool.h>

#define LOW_PRIO  (0<<0)
#define HIGH_PRIO (1<<0)

//ERROR_TYPE for status type
#define I2C_ERROR (1<<1)
#define GEN_ERROR (1<<1)
#define SERIAL_ERROR (2<<1)
#define NET_ERROR (3<<1)

//ERROR_DEVICE_NUMBER
#define ERROR_ENUM_STATE(x)	   ((x & 0x1F) << 1)
#define ERROR_DEVICE_NUMBER(x) (x << 6)

typedef enum{
	ERROR_NONE = 0x00,
	//RFID reader
	ERROR_RFID =  ERROR_DEVICE_NUMBER(1), //

	//QR reader
	ERROR_QR_I2C = ERROR_DEVICE_NUMBER(2)|I2C_ERROR, //

	//Server communication protocol
	ERROR_SCP = ERROR_DEVICE_NUMBER(3), //

	//4Bit io
	ERROR_IO_4BIT_I2C = ERROR_DEVICE_NUMBER(4)|I2C_ERROR,//

	//Network time protocol
	ERROR_RTC = ERROR_DEVICE_NUMBER(5)|GEN_ERROR, //

	//Network time protocol
	ERROR_NTP = ERROR_DEVICE_NUMBER(5)|NET_ERROR, //

	//Printer TG2460H
	ERROR_TG2460H = ERROR_DEVICE_NUMBER(6), //

	//ATM
	ERROR_CCV = ERROR_DEVICE_NUMBER(7), //

	//keypad
	ERROR_KEYPAD = ERROR_DEVICE_NUMBER(8)|I2C_ERROR, //

	//EMP800 coin acceptor
	ERROR_EMP800 = ERROR_DEVICE_NUMBER(9)|I2C_ERROR, //

	//Printer VK80
	ERROR_VK80 = ERROR_DEVICE_NUMBER(10), //

}error_number_t;

typedef enum{
	TYPE_STATUS = 0,
	TYPE_ENUM = 1,
}error_type_t;

typedef struct {
	struct error_list_item_t * next;
	error_type_t type;
	void  * error_var;
	error_number_t error_number;
	uint32_t enum_mask;
	uint32_t prio_mask;
	bool scp_send;
} error_list_item_t;

/*
 * @brief start sending errors to server
 */
void error_check_init(void);

/*
 * @brief add new status to check
 * @param type Type of error variable
 * @param error_var status variable
 * @param error_number error number to return when error is checked
 */
void error_check_add(error_type_t type,void * error_var ,error_number_t error_number);

/*
 * @brief return to begin of error check list
 */
void error_check_start(void);

/*
 * @brief return the next active error number (first run error_check_start to resturn to begin of list)
 * @return error number
 */
uint32_t error_check_next_error(void);

/*
 * @brief are there any errors?
 * @return true or false
 */
bool error_check_no_errors(void);

/*
 * @brief change the priority of a check
 * @param error_number check to change
 * @param prio_mask value to change the priority for status type LOW_PRIO or HIGH_PRIO
 * @return change succes true
 */
bool error_check_change_priority(error_number_t error_number,uint32_t prio_mask);

/*
 *@brief lower the priority of a error (high prio == out of order)
 *@param error_number check to change
 *@param enumval change prio of enum value. a status type error don't care about this value
 */
bool error_check_lower_priority(error_number_t error_number, uint8_t enumval);
/*
 * @brief ignore some values of a error enum
 * @param error_number check to change
 * @param enumval enum value to ignore
 */
bool error_check_ignore_enum_val(error_number_t error_number,uint8_t enumval);
#endif //__ERROR_CHECK_H__
 /**
 * 	@}
 * @}
 **/

