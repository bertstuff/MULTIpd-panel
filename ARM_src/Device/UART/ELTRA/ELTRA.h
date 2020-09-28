#ifndef ELTRA_H_
#define ELTRA_H_

#include <Core/Peripherals/UART_Dev_Init.h>
#include <Driver/include/lpc_types.h>
#include <stdint.h>
#include <Processes/Global_UID.h>
#include <Device/I2C/RFID/RFID.h>
#include "ELTRA_cmd.h"

extern process_event_t event_ELTRA;

typedef enum {
	CARTRIGE_FULL,
	CARTRIGE_NORMAL,
	CARTRIGE_LOW,
	CARTRIGE_EMPTY,
	CARTRIGE_JAM,
}ELTRA_cartrige_t;


/*
 * @brief init ELTRA device
 * @param Com uart select
 */
void ELTRA_init(UART_select_t Com);

/*
 * @brief cartrige state
 */
ELTRA_cartrige_t ELTRA_cartrige_state(void);

/*
 * @brief dispenser available
 * @return true when dispenser available
 */
bool ELTRA_dispenser_available(void);


/*
 * @brief Enable accepting cards
 * @param state true enable false disable
 */
void ELTRA_enable(bool state);

/*
 * @brief eject card
 * @param good true green led false red led
 */
void ELTRA_eject(bool good);

/*
 * @brief trash card
 * @param good true green led false red led
 */
void ELTRA_capture(bool good);

/*
 * @brief issue card from stk
 */
void ELTRA_issue_card(void);

/*
 * @brief return last error
 */
ELTRA_error_t ELTRA_last_error(void);

/*
 * @brief retrun current card
 */
ELTRA_card_t ELTRA_get_current_card(void);

ELTRA_card_t * ELTRA_get_current_reader(void);

bool ELTRA_card_removed(void);

#endif
