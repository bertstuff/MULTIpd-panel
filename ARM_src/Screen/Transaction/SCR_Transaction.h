#include <Board/Def_config.h>
#if (PAYING_MODULE == 1)
#ifndef __TRANSACTION_H__
#define __TRANSACTION_H__
	#include <Screen/Transaction/SCR_Transaction_products.h>
	#include <Core/Process/process.h>
	#include <Core/Thread/pt.h>
	#include <stdbool.h>
	#include <stdint.h>

	typedef enum{
		PAY_LOCAL,
		PAY_RESERVATION_ASK,
		PAY_RESERVATION,
		PAY_KNOWN_RESERVATION,
	}Transaction_type_t;

	typedef enum{
		//states
		RESERVATION_CANCELD,
		RESERVATION_YES,
		RESERVATION_NO,
		RESERVATION_OK,
		RESERVATION_PAYED

	}Transaction_reservation_t;

	typedef enum{
		STATE_PAY_METHOD_QUESTION,
		STATE_RECEIPT_QUESTION,
		STATE_PRINTING,
		STATE_ATM
	}Transaction_state_t;

	typedef struct{
		//--payment devices available--
		bool Payter_available;
		bool Cash_available;
		bool Coin_changer_available;
		bool Coin_acceptor_available;
		bool ATM_available;
		bool Multipass_available;
		bool Multipass_offline;
		bool Multipass_online;
		bool EltraRFID_available;
		bool QR_available;
		bool Printer_available;

		//--mandatory transaction information--
		Transaction_type_t Transaction_type;
		int32_t price;
		PT_THREAD((* Delivery_box)(struct pt * box_pt, bool *Delivery_ok, process_event_t ev, process_data_t data));
		PT_THREAD((* Transaction_Idle)(struct pt * idle_pt, bool * exit_transation,process_event_t ev, process_data_t data));
		Transaction_reservation_t reservation;
		bool payment_success;

		//Check machine availability
		bool check_availability;
		bool move_reservation;
		uint8_t machine_type;

		//return values
		uint8_t resv_pending;
		int32_t min_available;

		//--local variable--
		bool print_receipt;
		int32_t to_pay;
		int32_t to_return;
		int32_t payed;
		int32_t payed_SCP;
		int16_t multipass_payed;
		int16_t cash_payed;
		int16_t bank_payed;

	}Transaction_info_t;


	extern struct pt Transaction_pt;
	extern struct pt Transaction_sub_pt;
	extern Transaction_info_t * g_cur_trans;

	/*
	 * @brief main transaction thread
	 *
	 */
	PT_THREAD(Transaction_thread(Transaction_info_t * trans,process_event_t ev, process_data_t data));

	//local functions
	PT_THREAD(Transaction_Delivery_box(struct pt * box_pt, bool *Delivery_ok, process_event_t ev, process_data_t data));
	PT_THREAD(Disable_devices(Transaction_info_t * trans, process_event_t ev, process_data_t data));
#endif //__TRANSACTION_H__
#endif
