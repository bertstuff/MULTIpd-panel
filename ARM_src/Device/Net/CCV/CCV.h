#ifndef CCV_H_
#define CCV_H_

#include "CCV_global.h"

extern process_event_t event_CCV;
PROCESS_NAME(CCV_Startup_process);
PROCESS_NAME(CCV_Chan1_process);


//------------User global API functions------------//

#define CCV_enabled (process_is_running(&CCV_Chan1_process) != 0)

/*
 * @brief init CCV ATM
 * when called the variable cur_payment contains information about the current payment
 * @return void
 */
void CCV_init(void);
bool CCV_startup_done(void);
/*
 * @brief create new payment
 * @param DeliveryBox function to call for delivering the product. a thread function with params:
 *  -box_pt use this thread handle
 *  -Delivery_status status pointer to return
 *  -ev & data process event data
 * @return void
 */
void CCV_CardPayment(PT_THREAD((* Delivery_box)(struct pt * box_pt, bool *Delivery_ok, process_event_t ev, process_data_t data)),bool print_receipt);

/*
 * @brief refund payment
 * @param DeliveryBox function to call for delivering the product. a thread function with params:
 *  -box_pt use this thread handle
 *  -Delivery_status status pointer to return
 *  -ev & data process event data
 * @return void
 */
void CCV_PaymentRefund(PT_THREAD((* Delivery_box)(struct pt * box_pt, bool *Delivery_ok, process_event_t ev, process_data_t data)));

/*
 * @brief Abort current payment
 */
void CCV_AbortRequest(void);

/*
 * @brief reprint last receipt
 * @return void
 */
void CCV_Reprint_Receipt(void);


/*
 * @brief This function returns true when a payment is started.
 *  A power down or reset won't reset this flag. used for indicating a unwanted power down
 * @return true when payment is started
 */
bool CCV_Get_PaymentStartedFlag(void);

/*
 * @brief get current status of the ATM
 * @return status
 */
CCV_OverallResult_t CCV_Get_OverallResult(void);

/*
 * @brief get accepted card brands
 * @return card brand list
 */
CCV_CardBrand_t CCV_Get_CardBrand(void);

/*
 * @brief increase shift number call when execute a Daily cash register closure
 */
void CCV_increase_ShiftNumber(void);

/*
 * @brief return Shiftnumber
 */
uint16_t CCV_get_ShiftNumber(void);
#endif /* CCV_H_ */

