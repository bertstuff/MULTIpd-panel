/**
 * \addtogroup device
 * @{
 */

/**
 * \defgroup XBA_API
 *
 * bill acceptor api
 *
 * @{
 */

/**
 * \file
 * This file contains functions for the XBA
 * \author
 *         Mark van der Heijden <m.vanderheijden@custommate.eu>
 *
 */
#ifndef XBA_H_
#define XBA_H_
	#include <stdint.h>
	#include <stdbool.h>
	#include <string.h>
	#include <stdio.h>
	#include <Driver/include/lpc17xx_uart.h>
	#include <Core/Peripherals/UART_Dev_Init.h>

	typedef enum{
		E_XBA_NONE,
		E_XBA_MOTOR_FAILURE,
		E_XBA_CHECKSUM_ERROR,
		E_XBA_BILL_JAM,
		E_XBA_STACKER_OPEN,
		E_XBA_SENSOR_PROBLEM,
		E_XBA_BILL_FISH,
		E_XBA_STACKER_PROBLEM,
		E_XBA_BILL_REJECT,
		E_XBA_INVALID_COMMAND,
	}XBA_error_t;


 	/**
 	 * Event when a bill is validated data pointer to last bill value
 	 * */
	extern process_event_t event_Bill_validated;

	/**
 	 * error event. data point to last error
 	 * */
	extern process_event_t event_Bill_error;

	/**
	 * @brief init bill validator
	 * @param Com uart to use
	 */
	void XBA_init(UART_select_t Com);

	/**
	 * @brief Answer to event_Bill_validated. hold bill
	 */
	void XBA_Bill_Hold_in_escrow(void);

	/**
	 * @brief Answer to event_Bill_validated. accept bill
	 */
	void XBA_Bill_Accept(void);

	/**
	 * @brief Answer to event_Bill_validated. reject bill
	 */
	void XBA_Bill_reject(void);

	/**
	 * @brief enable Bill validator
	 */
	void XBA_Enable(void);

	/**
	 * @brief disbale Bill validator
	 */
	void XBA_Disable(void);

	/**
	 * @brief Reset Bill validator
	 */
	void XBA_Reset(void);
#endif
 /**
 * 	@}
 * @}
 **/

