#ifndef CCV_CHAN1_H_
#define CCV_CHAN1_H_

#include "CCV_global.h"


 PROCESS_NAME(CCV_Chan1_process);

 /*
  * @brief This function returns true when a payment is started.
  *  A power down or reset won't reset this flag. used for indicating a unwanted power down
  * @return true when payment is started
  */
 bool CCV_Get_PaymentStartedFlag(void);
 void CCV_Set_PaymentStartedFlag(bool state);

 /*
  * @repeat last message
  */
 void CCV_RepeatLastMessage(void);

 /*
  * @brief get accepted card brands
  */
 void CCV_GetCardCircuits(void);

#endif /* CCV_CHAN1_H_ */

