#include <Board/Def_config.h>
#ifndef __TRANSACTION_RECEIPT_H__
#define __TRANSACTION_RECEIPT_H__
#if (PAYING_MODULE == 1)

	#include <Screen/Transaction/SCR_Transaction.h>
	#include <Core/Process/process.h>

	void Receipt_init(void);

	void Receipt_header(void);

	void Receipt_Sales_lines(void);

	void Receipt_footer(void);

	void Receipt_transaction_canceled(Transaction_info_t * trans);

	void Receipt_daily_cash_register_closure(Devices_available_t devices_available);

	void Receipt_Cabinet_opened(Transaction_info_t * trans);

	void SCR_printing_receipt(void);

	void SCR_receipt_transaction(void);

	void Receipt_transaction_accept(Transaction_info_t * trans);

	PT_THREAD(Quest_no_paper(Transaction_info_t * trans, process_event_t ev, process_data_t data));

	PT_THREAD(Quest_print_receipt(Transaction_info_t * trans, process_event_t ev, process_data_t data));
#endif

#endif //__TRANSACTION_RECEIPT_H__
