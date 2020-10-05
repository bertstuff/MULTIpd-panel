#include <Board/Def_config.h>
#if (PAYING_MODULE == 1)
#ifndef __TRANSACTION_STATISTICS_H__
#define __TRANSACTION_STATISTICS_H__
	#include <stdint.h>
	#include <time.h>

	typedef struct{
		int32_t Coins_payed;
		int32_t Coins_return;
		int32_t Coins_in_cash_box;
		int32_t Cash_payed;
		int32_t Debit_card_payed;
		int32_t Multipass_payed;
		uint32_t Reservations;
		uint32_t Transactions;
		uint32_t Counter;
		struct tm Last_reset;
	}Transaction_statistics_t;

	extern Transaction_statistics_t g_trans_statistics;


	void Transaction_statistics_load(void);
	void Transaction_statistics_save(void);

#endif //__TRANSACTION_STATISTICS_H__
#endif
