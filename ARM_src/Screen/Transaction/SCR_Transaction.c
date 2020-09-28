#include <Board/Def_config.h>

#if (PAYING_MODULE == 1)
#include <Core/Clock/clock.h>
#include <Core/Clock/rtc_cal.h>
#include <core/Clock/Time_functions.h>
#include <Core/Extensions/ini/minIni.h>
#include <Core/Thread/pt.h>
#include <Device/I2C/Edip_240B/EDIP_240B.h>
#include <stdio.h>
#include <string.h>
#include <Screen/SCR_Init.h>
#include <Core/protocol/Net/SCP/SCP_Comm.h>
#include <core/protocol/Net/SCP/SCP_msg.h>
#include <Core/protocol/Net/SCP/tRFIDmsg.h>
#include <Core/Process/process.h>
#include <Core/Timer/etimer.h>
#include <Core/Timer/timer_functions.h>
#include <Core/Type/type.h>
#include  <Device/I2C/BUTTONS/Keypad_Parser.h>
#include <Device/I2C/Edip_240B/EDIP_240B_IO.h>
#include <Device/I2C/Edip_240B/EDIP_240B_Display.h>
#include <Device/I2C/Edip_240B/EDIP_240B_Draw.h>
#include <Device/I2C/Edip_240B/EDIP_240B_Text.h>
#include <Device/I2C/Edip_240B/EDIP_240B_Types.h>
#include <Device/I2C/LV3095/LV3095.h>
#include <Device/I2C/RFID/RFID.h>
#include <Device/I2C/EMP_800/EMP_800.h>
#include <Device/Net/CCV/CCV_global.h>
#include <Device/Net/payter/RTP_types.h>
#include <Device/Net/payter/RTP.h>
#include <Device/Net/payter/RTP_msg.h>
#include <Device/Net/CCV/CCV.h>
#include <Device/UART/MDB-C21/MDB-ARB-C21-7TUBES.h>
#include <Device/UART/XBA/XBA.h>
#include <Device/UART/TG2460H/TG2460H.h>
#include <Device/IO/BUTTONS/EBM-Basismodule-buttons.h>
#include <Global.h>
#include <Screen/SCR_Text.h>
#include <Screen/SCR_Message.h>
#include <Screen/SCR_Text.h>
#include <Processes/Global_UID.h>
#include <Screen/Transaction/SCR_Transaction.h>
#include <Screen/Transaction/SCR_Transaction_init.h>
#include <Screen/Transaction/SCR_Transaction_products.h>
#include <Screen/Transaction/SCR_Transaction_ATM.h>
#include <Screen/Transaction/SCR_Transaction_Reservation.h>
#include <Screen/Transaction/SCR_Transaction_Receipt.h>
#include <Screen/Transaction/SCR_Transaction_statistics.h>
#include <Core/Util/math_util.h>
#include <Processes/Reservation.h>

#define SCP_PAYED_THRESHOLD 500

typedef enum{
	CANCEL_METHOD,
	DEBIT_CARD_METHOD,
	CHASH_METHOD
}Transaction_method_t;

struct pt pay_pt;
struct pt Transaction_sub_pt;
struct pt Transaction_pt;
Transaction_statistics_t g_trans_statistics;
bool g_bill_accept[4];
int32_t g_min_change_level[4];
int32_t g_max_change_value;
bool Delivery_ok;
Transaction_info_t * g_cur_trans;
extern uint8_t pay_event;
bool Devices_enabled;

SCP_Data_t send_CashStatus(void);
bool check_bill_accept(uint16_t centen);
bool check_cash_level(Transaction_info_t * trans, uint16_t centen);
void SCR_pay(Transaction_info_t * trans, bool refresh_all);
void SCR_Return(void);
void SCR_Choose_payment_method(void);
bool Trans_state_change(Transaction_state_t state) __attribute__((weak));

/*
 * @brief disable all devices
 */
PT_THREAD(Disable_devices(Transaction_info_t * trans, process_event_t ev, process_data_t data)){
	static RTP_packet_t RTP_packet;

	PT_BEGIN(&pay_pt);
	Devices_enabled = false;
	if(trans->Device_available.Coin_changer_available){
		MDB_Ontvang(false);
	}

	if(trans->Device_available.Cash_available){
		XBA_Disable();
	}

	if(trans->Device_available.Payter_available){
		PT_PTR_MSG_EMV_CANCEL(&pay_pt,&RTP_packet);
		RTP_Free_packet(&RTP_packet);
	}

	if(trans->Device_available.QR_available){
		QR_enable(false);
	}

	if(trans->Device_available.Coin_acceptor_available){
		EMP_800_set_blocking(true);
	}
	PT_END(&pay_pt);
}

/*
 * @brief enable all devices
 */
PT_THREAD(Enable_devices(Transaction_info_t * trans, process_event_t ev, process_data_t data)){
	static RTP_packet_t RTP_packet;

	PT_BEGIN(&pay_pt);
	Devices_enabled = true;

	if((trans->Device_available.Payter_available) && (trans->to_pay > 0)){
		PT_SPAWN(&pay_pt,&RTP_packet.p,PTR_msg_EMV_Transaction(&RTP_packet,trans->to_pay));
		RTP_Free_packet(&RTP_packet);
	}

	if(trans->Device_available.Coin_changer_available){
		MDB_Ontvang(true);
	}

	if(trans->Device_available.Cash_available){
		XBA_Enable();
	}

	if(trans->Device_available.QR_available){
		QR_enable(true);
	}

	if(trans->Device_available.Coin_acceptor_available){
		EMP_800_set_blocking(false);
	}
	PT_END(&pay_pt);
}

/*
 * @brief cancel Cash payment
 */
PT_THREAD(Cancel_transaction(Transaction_info_t * trans, process_event_t ev, process_data_t data)){

	PT_BEGIN(&Transaction_sub_pt);
	PT_SPAWN(&Transaction_sub_pt,&pay_pt,Disable_devices(trans, ev, data));

#if(RESERVATION_MODULE == 1)
	SCP_pack_t * SCP_packet;
	if((trans->reservation == RESERVATION_OK) && (trans->payed > 0)){
		Please_wait_screen(true);
		PT_SCP_MSG_RESERVERINGBETAALD(&Transaction_sub_pt,&SCP_packet,g_Cur_ResvNr,trans->payed);
		if(SCP_packet->Data.Message_type == msg_Ok){
			trans->payed_SCP += trans->payed;
			trans->payed = 0;
		}
		Please_wait_screen(false);
	}
#endif
	trans->to_return = trans->payed;
	printf("pay:Cancel transaction return %d cents\r\n",trans->to_return);
	PT_END(&Transaction_sub_pt);
}

/*
 * @ delivery box
 */
PT_THREAD(Transaction_Delivery_box(struct pt * box_pt, bool *Delivery_ok, process_event_t ev, process_data_t data)){
	Transaction_info_t * trans = g_cur_trans;
	static struct pt Delivery_box_pt;

	PT_BEGIN(box_pt);
	printf("----------SCP Delivery_box started----------------\r\n");
#if(RESERVATION_MODULE == 1)
	SCP_pack_t * SCP_packet;
	if(trans->reservation == RESERVATION_OK){
		Please_wait_screen(true);
		printf("pay: Send %d cents to server\r\n",trans->payed + trans->to_pay);
		PT_SCP_MSG_RESERVERINGBETAALD(box_pt,&SCP_packet,g_Cur_ResvNr,(trans->payed + trans->to_pay));
		Please_wait_screen(false);
		if(!(SCP_packet->Data.Message_type == msg_Ok)){
			*Delivery_ok = false;
			PT_EXIT(box_pt);
		}
	}
#endif

	PT_SPAWN(box_pt, &Delivery_box_pt,(trans->Delivery_box)(&Delivery_box_pt,Delivery_ok, ev, data));

	if(*Delivery_ok == true){
		g_trans_statistics.Debit_card_payed += trans->to_pay;
		trans->bank_payed += trans->to_pay;
		Transaction_statistics_save();
		trans->payment_success = true;
		trans->payed += trans->to_pay;
#if(RESERVATION_MODULE == 1)
		if(trans->reservation == RESERVATION_OK){
			trans->payed_SCP += trans->payed;
			trans->payed = 0;
		}
	}else{
		if(trans->reservation == RESERVATION_OK){
			BUFFER_SCP_MSG_RESERVERINGBETAALD(g_Cur_ResvNr,-(trans->payed + trans->to_pay));
		}
#endif
	}
	PT_END(box_pt);
}


/*
 * @brief let the user select a payment method
 */
PT_THREAD(Choose_payment_method(Transaction_info_t * trans, Transaction_method_t * method, process_event_t ev, process_data_t data)){
	TG2460H_Paper_status_t Paper_status;
	PT_BEGIN(&Transaction_sub_pt);
	if( !trans->Device_available.Cash_available &&
		!trans->Device_available.Coin_acceptor_available &&
		!trans->Device_available.Coin_changer_available &&
		!trans->Device_available.Multipass_available &&
		!trans->Device_available.Payter_available &&
		!trans->Device_available.QR_available
	){
		Paper_status = TG2460H_Get_paper_status();
		if(Paper_status == S_PAPER_EMPTY){
			PT_MESSAGE_SCREEN(&Transaction_sub_pt,GET_TEXT(S_Receipt_paper_empty));
			PT_MESSAGE_SCREEN(&Transaction_sub_pt,GET_TEXT(S_Please_warn_an_employee));
		}else if(Paper_status == S_DEVICE_UNAVAILABLE){
			PT_MESSAGE_SCREEN(&Transaction_sub_pt,GET_TEXT(S_Printer_error));
			PT_MESSAGE_SCREEN(&Transaction_sub_pt,GET_TEXT(S_Please_warn_an_employee));
		}else if(CCV_startup_done() == false){
			PT_MESSAGE_SCREEN(&Transaction_sub_pt,GET_TEXT(S_ATM_error));
			PT_MESSAGE_SCREEN(&Transaction_sub_pt,GET_TEXT(S_Please_warn_an_employee));
		}
		*method = DEBIT_CARD_METHOD;
		PT_EXIT(&Transaction_sub_pt);
	}
	if(Trans_state_change){
		Trans_state_change(STATE_PAY_METHOD_QUESTION);
	}
	SCR_Choose_payment_method();
	SET_SCREEN_TIMEOUT(20);

	while(ev != PROCESS_EVENT_EXIT){
		//--keypad--
		if(PROCESS_EVENT_SCREEN_TIMEOUT){
			*method = CANCEL_METHOD;
			PT_EXIT(&Transaction_sub_pt);
		}

		if(keypad_pressed(ev)){
			uint8_t key = *((uint8_t *)data);
			if(key == C_RETURN){
				*method = CANCEL_METHOD;
				PT_EXIT(&Transaction_sub_pt);
			}else if(key == 1){
				Paper_status = TG2460H_Get_paper_status();
				if(Paper_status == S_PAPER_EMPTY){
					PT_MESSAGE_SCREEN(&Transaction_sub_pt,GET_TEXT(S_Receipt_paper_empty));
					PT_MESSAGE_SCREEN(&Transaction_sub_pt,GET_TEXT(S_Please_warn_an_employee));
				}else if(Paper_status == S_DEVICE_UNAVAILABLE){
					PT_MESSAGE_SCREEN(&Transaction_sub_pt,GET_TEXT(S_Printer_error));
					PT_MESSAGE_SCREEN(&Transaction_sub_pt,GET_TEXT(S_Please_warn_an_employee));
				}else if(CCV_startup_done() == false){
					PT_MESSAGE_SCREEN(&Transaction_sub_pt,GET_TEXT(S_ATM_error));
					PT_MESSAGE_SCREEN(&Transaction_sub_pt,GET_TEXT(S_Please_warn_an_employee));
				}else{
					*method = DEBIT_CARD_METHOD;
					PT_EXIT(&Transaction_sub_pt);
				}
				SCR_Choose_payment_method();
			}else if(key == 2){
				*method = CHASH_METHOD;
				PT_EXIT(&Transaction_sub_pt);
			}
		}
		PT_YIELD(&Transaction_sub_pt);
	}
	PT_END(&Transaction_sub_pt);
}

/*
 * @ brief main transaction thread
 */
PT_THREAD(Transaction_thread(Transaction_info_t * trans,process_event_t ev, process_data_t data)){
	static RTP_packet_t RTP_packet;
	SCP_pack_t * SCP_packet;
	static struct etimer timeout_tm;
	Transaction_method_t Transaction_method;
	TG2460H_Paper_status_t Paper_status;

	PT_BEGIN(&Transaction_pt);
	print_datetime();
	//--------------------------------------------start init transaction--------------------------------------------
	printf("Transaction: start init transaction \r\n");
	if(trans->reservation == RESERVATION_NO){
		if(Productlist_total_price(&trans->price) == false){
			printf("pay: No product list found\r\n");
		}
	}

	Devices_enabled = false;
	g_cur_trans = trans;
	trans->to_pay = trans->price;
	trans->to_return = 0;
	trans->payed = 0;
	trans->payed_SCP = 0;
	trans->payment_success = false;
	trans->multipass_payed = 0;
	trans->cash_payed = 0;
	trans->bank_payed = 0;
	Delivery_ok = false;

	if(trans->to_pay == 0){
		PT_SPAWN(&Transaction_pt, &Transaction_sub_pt,Transaction_Delivery_box(&Transaction_sub_pt,&trans->payment_success, ev, data));
		if(trans->payment_success == true){
			goto Print_receipt_label;
		}
	}


	//choose payment methode
	if(trans->Device_available.ATM_available){
		printf("Transaction: choose payment method\r\n");
		PT_SPAWN(&Transaction_pt,&Transaction_sub_pt,Choose_payment_method(trans, &Transaction_method, ev, data));
		if(Transaction_method == CANCEL_METHOD){
			Productlist_clear();
			PT_EXIT(&Transaction_pt);
		}else if(Transaction_method == DEBIT_CARD_METHOD){
			// --- ATM payment ---
			//print receipt?
			Paper_status = TG2460H_Get_paper_status();
			if(Paper_status == S_PAPER_OK){
				PT_SPAWN(&Transaction_pt,&Transaction_sub_pt,Quest_print_receipt(trans, ev, data));
			}else{
				PT_SPAWN(&Transaction_pt,&Transaction_sub_pt,Quest_no_paper(trans, ev, data));
			}
			if(trans->payment_success == false){
				//screen timeout or canceld
				Productlist_clear();
				PT_EXIT(&Transaction_pt);
			}
			//start ATM transaction
			PT_SPAWN(&Transaction_pt, &ATM_pt, ATM_thread(trans, ev, data));
			trans->bank_payed = trans->payed;
			BUFFER_SCP_MSG_TRANSACTION_DONE(0, 0, trans->bank_payed);
			BUFFER_SCP_MSG_CASHSTATUS(send_CashStatus());
			Productlist_clear();
			PT_EXIT(&Transaction_pt);
		}
	}

	//---Cash payements----

	//print receipt?
	if(trans->Device_available.Printer_available){
		Paper_status = TG2460H_Get_paper_status();
		if(Paper_status == S_PAPER_OK){
			//print receipt question
			PT_SPAWN(&Transaction_pt,&Transaction_sub_pt,Quest_print_receipt(trans, ev, data));
		}else{
			//paper empty
			PT_SPAWN(&Transaction_pt,&Transaction_sub_pt,Quest_no_paper(trans, ev, data));
		}
		if(trans->payment_success == false){
			//timeout or return button
			Productlist_clear();
			PT_EXIT(&Transaction_pt);
		}
		trans->payment_success = false;
	}

	//device init
	if(trans->Device_available.Payter_available){
		PT_PTR_MSG_EMV_TRANSACTION(&Transaction_pt, &RTP_packet, trans->to_pay);
		RTP_Free_packet(&RTP_packet);
	}

	if(trans->Device_available.Cash_available){
		XBA_Enable();
	}

	if(trans->Device_available.Coin_changer_available){
		MDB_Ontvang(true);
	}

	if(trans->Device_available.QR_available){
		QR_enable(true);
	}

	if(trans->Device_available.Coin_acceptor_available){
		EMP_800_enable();

		int32_t coin_added = EMP_800_get_value_added();
		trans->payed += coin_added;
		trans->cash_payed += coin_added;
	}

	//--------------------------------------------receive money--------------------------------------------
	SET_SCREEN_TIMEOUT(60);
	SCR_pay(trans,true);
	while(ev != PROCESS_EVENT_EXIT){

		//custom_handler
		if(trans->Transaction_Idle != NULL){
			bool exit_transation;
			PT_SPAWN(&Transaction_pt, &pay_pt,(trans->Transaction_Idle)(&pay_pt,&exit_transation,ev, data));
			if(exit_transation == true){
				PT_SPAWN(&Transaction_pt,&Transaction_sub_pt,Cancel_transaction(trans, ev, data));
				break;
			}
		}

		//--Timeout--
		if(PROCESS_EVENT_SCREEN_TIMEOUT){
			if(trans->payed == 0){
				PT_SPAWN(&Transaction_pt,&Transaction_sub_pt,Cancel_transaction(trans, ev, data));
			}
			break;
		}


		//--keypad--
		if(ev == event_Button3x4_pressed){
			uint8_t key = *((uint8_t *)data);
			if(key == C_RETURN){ //Cancel key
				PT_SPAWN(&Transaction_pt,&Transaction_sub_pt,Cancel_transaction(trans, ev, data));
				break;
			}
		}

		//--Coins--
		if(trans->Device_available.Coin_changer_available){
			if(ev == event_coin_inserted){
				SET_SCREEN_TIMEOUT(60);

				trans->payed += *((uint16_t *)data);
				printf("pay:received %d cents\r\n",*((uint16_t *)data));
				g_trans_statistics.Coins_payed += *((uint16_t *)data);
				trans->cash_payed += *((uint16_t *)data);
				*((uint16_t *)data) = 0;

				g_trans_statistics.Coins_in_cash_box += (Cash_Box);
				Cash_Box = 0;
				Transaction_statistics_save();

				ev = event_coin_inserted;
				SCR_pay(trans,false);
				PT_SPAWN(&Transaction_pt,&pay_pt,Disable_devices(trans, ev, data));
			}
		}

		if(trans->Device_available.Coin_acceptor_available){
			if(ev == EMP_800_event){
				int32_t coin_difference = EMP_800_get_value_added();
				trans->payed += coin_difference;
				trans->cash_payed += coin_difference;
				printf("pay:received %d cents\r\n",coin_difference);
				g_trans_statistics.Coins_payed += coin_difference;
				g_trans_statistics.Coins_in_cash_box += coin_difference;
				Transaction_statistics_save();
				SCR_pay(trans,false);
				SET_SCREEN_TIMEOUT(3600);
				PT_SPAWN(&Transaction_pt,&pay_pt,Disable_devices(trans, ev, data));
			}
		}

		//--paper money--
		if(trans->Device_available.Cash_available){
			if(ev == event_Bill_validated){
				static uint16_t bill_value;
				SET_SCREEN_TIMEOUT(60);
				bill_value = *((uint16_t *)data);
				printf("pay:received %d cents\r\n",bill_value);
				if(bill_value - (trans->price - (trans->payed + trans->payed_SCP)) >= g_max_change_value){
					XBA_Bill_reject();
					PT_SPAWN(&Transaction_pt,&pay_pt,Disable_devices(trans, ev, data));
					PT_MESSAGE_SCREEN(&Transaction_pt,GET_TEXT(S_Please_pay_smaller));
					goto bill_failed_label;
				}
				if(check_bill_accept(bill_value) == false){
					XBA_Bill_reject();
					PT_SPAWN(&Transaction_pt,&pay_pt,Disable_devices(trans, ev, data));
					PT_MESSAGE_SCREEN(&Transaction_pt,GET_TEXT(S_Bill_value_not_accepted));
					goto bill_failed_label;
				}
				if(check_cash_level(trans, bill_value) == false){
					XBA_Bill_reject();
					PT_SPAWN(&Transaction_pt,&pay_pt,Disable_devices(trans, ev, data));
					PT_MESSAGE_SCREEN(&Transaction_pt,GET_TEXT(S_no_change));
					goto bill_failed_label;
				}

				XBA_Bill_Accept();
				trans->payed += bill_value;
				trans->cash_payed += bill_value;
				g_trans_statistics.Cash_payed += bill_value;
				Transaction_statistics_save();
				PT_SPAWN(&Transaction_pt,&pay_pt,Disable_devices(trans, ev, data));
				bill_failed_label:
				SCR_pay(trans,true);
			}

			if(ev == event_Bill_error){
				static XBA_error_t XBA_error = E_XBA_NONE;
				XBA_error = *((XBA_error_t *)data);
				PT_SPAWN(&Transaction_pt, &pay_pt, Disable_devices(trans, ev, data));
				if( (XBA_error == E_XBA_MOTOR_FAILURE)||
					(XBA_error ==  E_XBA_CHECKSUM_ERROR)||
					(XBA_error ==  E_XBA_BILL_JAM)||
					(XBA_error ==  E_XBA_SENSOR_PROBLEM)||
					(XBA_error ==  E_XBA_STACKER_PROBLEM)||
					(XBA_error ==  E_XBA_INVALID_COMMAND)
				){
					PT_MESSAGE_SCREEN(&Transaction_pt,GET_TEXT(S_pay_diffrent));
				}else if(XBA_error ==  E_XBA_BILL_FISH){
					Edip_Set_Alarm_Buzzer(true);
					PT_MESSAGE_SCREEN(&Transaction_pt,GET_TEXT(S_Intrusion_detected));
					Edip_Set_Alarm_Buzzer(false);
				}else if(XBA_error ==  E_XBA_BILL_REJECT){
					PT_MESSAGE_SCREEN(&Transaction_pt,GET_TEXT(S_Bill_not_accepted));
				}
				SCR_pay(trans,true);
			}
		}


		//--Payter--
		if(trans->Device_available.Payter_available){
			if(ev == event_RTP){//handel payter event
				bool mifare;
				RTP_packet_t * packet = data;

				if(packet->received_frame.CmdID == EMV_TRANSACTION){
					if(packet->status == S_OK){
						SET_SCREEN_TIMEOUT(60);
						Please_wait_screen(true);
						UID_t PTR_UID;
						mifare = PTR_EMV_Transaction_event_Get_UID(packet, &PTR_UID);
						RTP_Free_packet(packet);
						//-multipass card-
						if((mifare == true) && (trans->Device_available.Multipass_available)){
							Set_GLobal_UID(PTR_UID);
							goto Mifare_label;
						//-Debit card-
						}else if(mifare == false){
							PT_SPAWN(&Transaction_pt, &Transaction_sub_pt,Transaction_Delivery_box(&Transaction_sub_pt,&trans->payment_success, ev, data));
							PT_PTR_MSG_EMV_CLEARING(&Transaction_pt, &RTP_packet, trans->payment_success);
							RTP_Free_packet(&RTP_packet);

							if((trans->payment_success == true) && (trans->Device_available.Coin_acceptor_available == true)){
								EMP_800_reset_value();
							}
							if(trans->payment_success == false){
								PT_MESSAGE_SCREEN(&Transaction_pt,GET_TEXT(S_Credit_updated_failed));
							}
							PT_SPAWN(&Transaction_pt,&pay_pt,Disable_devices(trans, ev, data));
							if(trans->payment_success == true){
								trans->bank_payed += RTP_transaction_cost(trans->bank_payed);
								goto Print_receipt_label;
							}
							SCR_pay(trans,true);
						}
					}else{
						RTP_Free_packet(packet);
						PT_MESSAGE_SCREEN(&Transaction_pt,GET_TEXT(S_Credit_updated_failed));
						SCR_pay(trans,true);
					}
				}else{
					RTP_Free_packet(packet);
				}

				Please_wait_screen(false);
			}
		}

		//--QR-code--
		if(trans->Device_available.QR_available){
			if(ev == event_QR_present){
				SET_SCREEN_TIMEOUT(60);
				Please_wait_screen(true);
				QR_User_t QR_user = QR_get_current_user();
				Set_GLobal_UID(QR_user.UID);
				PT_SPAWN(&Transaction_pt,&pay_pt,Disable_devices(trans, ev, data));
				PT_SCP_MSG_MELDEURO(&Transaction_pt, &SCP_packet,g_Cur_UID);
				if(SCP_packet->Data.Message_type == msg_QR_HuidigEuro){
					if(SCP_msg_HuidigEuro__Centen(SCP_packet) >= trans->to_pay){
						PT_SCP_MSG_AFEURO(&Transaction_pt, &SCP_packet,g_Cur_UID,trans->to_pay);
						if(SCP_packet->Data.Message_type == msg_QR_AfEuroOk){
							data = (void *)trans;
							PT_SPAWN(&Transaction_pt, &Transaction_sub_pt,(trans->Delivery_box)(&Transaction_sub_pt,&Delivery_ok, ev, data));
							if(Delivery_ok == true){
								trans->payment_success = true;
								trans->payed += trans->to_pay;
								trans->multipass_payed += trans->to_pay;
								g_trans_statistics.Multipass_payed += trans->to_pay;
								Transaction_statistics_save();
							}else{
								BUFFER_SCP_MSG_BIJEURO_IMPORTANT(g_Cur_UID, trans->to_pay, REASON_ROLLBACK);
							}
						}else if(SCP_packet->Data.Message_type == msg_NietAkkoord){
							PT_MESSAGE_SCREEN(&Transaction_pt,GET_TEXT(S_not_agreed));
						}else{
							printf("SCP: Error\r\n");
							PT_MESSAGE_SCREEN(&Transaction_pt,GET_TEXT(S_Server_error));
						}
					}else{
						printf("SCP: credit to low\r\n");
						PT_MESSAGE_SCREEN(&Transaction_pt,GET_TEXT(S_credit_low));
					}
				}else if(SCP_packet->Data.Message_type == msg_Onbekend){
					printf("SCP: card unknown\r\n");
					PT_MESSAGE_SCREEN(&Transaction_pt,GET_TEXT(S_Ticket_unknown));
				}else{
					printf("SCP: Error\r\n");
					PT_MESSAGE_SCREEN(&Transaction_pt,GET_TEXT(S_Server_error));
				}
				SCR_pay(trans,true);
			}
		}

		//--Multipass--
		if(trans->Device_available.Multipass_available){
			if((ev == event_card_change)||(ev == PROCESS_EVENT_INIT)){
				SET_SCREEN_TIMEOUT(60);
				g_RFID_card = RFID_get_card(RFID_get_reader(0x00));
				if(g_RFID_card->card_present == true){
					Please_wait_screen(true);

					//Read language from pass
					if(g_RFID_card->card_info.Language == NOT_SET){
						printf("RFID: No language selected on pass \r\n");
						g_Language = g_Language_def;
					}else{
						printf("RFID: language on pass %d\r\n",g_RFID_card->card_info.Language);
						g_Language = g_RFID_card->card_info.Language;
					}

					//Check card status on server
					memmove(&g_Cur_UID,&g_RFID_card->card_action,7);
					Mifare_label://jump place for Payter (mifare)
					PT_SPAWN(&Transaction_pt,&pay_pt,Disable_devices(trans, ev, data));
					Please_wait_screen(true);
					PT_SCP_MSG_MELDEURO(&Transaction_pt, &SCP_packet,g_Cur_UID);
					if(SCP_packet->Data.Message_type == msg_RFID_HuidigEuro){
						if(SCP_msg_HuidigEuro__Centen(SCP_packet) >= trans->to_pay){
							PT_SCP_MSG_AFEURO(&Transaction_pt, &SCP_packet,g_Cur_UID,trans->to_pay);
							if(SCP_packet->Data.Message_type == msg_RFID_AfEuroOk){
								data = (void *)trans;
								PT_SPAWN(&Transaction_pt, &Transaction_sub_pt,(trans->Delivery_box)(&Transaction_sub_pt,&Delivery_ok, ev, data));
								if(Delivery_ok == true){
									trans->payment_success = true;
									trans->payed += trans->to_pay;
									trans->multipass_payed += trans->to_pay;
									g_trans_statistics.Multipass_payed += trans->to_pay;
									Transaction_statistics_save();
								}else{
									BUFFER_SCP_MSG_BIJEURO_IMPORTANT(g_Cur_UID, trans->to_pay, REASON_ROLLBACK);
								}
							}else if(SCP_packet->Data.Message_type == msg_NietAkkoord){
								PT_MESSAGE_SCREEN(&Transaction_pt,GET_TEXT(S_not_agreed));
							}else{
								printf("SCP: Error\r\n");
								PT_MESSAGE_SCREEN(&Transaction_pt,GET_TEXT(S_Server_error));
							}
						}else{
							printf("SCP: credit to low\r\n");
							PT_MESSAGE_SCREEN(&Transaction_pt,GET_TEXT(S_credit_low));
						}
					}else if(SCP_packet->Data.Message_type == msg_Onbekend){
						PT_MESSAGE_SCREEN(&Transaction_pt,GET_TEXT(S_Pass_not_valid));
					}else if(SCP_packet->Data.Message_type == msg_Blokkade){
						PT_MESSAGE_SCREEN(&Transaction_pt,GET_TEXT(S_Pass_blocked));
					}else{
						printf("SCP: Error\r\n");
						PT_MESSAGE_SCREEN(&Transaction_pt,GET_TEXT(S_Server_error));
					}
				SCR_pay(trans,true);
				}
			}
		}

		if(ev == pay_event){
			int32_t event_centen;
			event_centen = *((uint16_t *)data);
			trans->payed += event_centen;
			trans->cash_payed += event_centen;
			SCR_pay(trans,true);
			PT_SPAWN(&Transaction_pt,&pay_pt,Disable_devices(trans, ev, data));
		}

		//--Calc change--
		trans->to_pay = (trans->price - (trans->payed + trans->payed_SCP));

		if(trans->to_pay <= 0){
			trans->to_pay = 0;
			//paid. start return money
			trans->to_return = ((trans->payed + trans->payed_SCP) - trans->price);
			PT_SPAWN(&Transaction_pt,&pay_pt,Disable_devices(trans, ev, data));
			Button3x4_activate(false);
			break;
		}else{
			//not paid wait for more money...

#if(RESERVATION_MODULE == 1)
			//send money above the SCP_PAYED_THRESHOLD to the server
			int32_t send_to_server;
			if(trans->reservation == RESERVATION_OK){
				if(trans->payed >= SCP_PAYED_THRESHOLD){
					PT_SPAWN(&Transaction_pt,&pay_pt,Disable_devices(trans, ev, data));
					printf("pay: Send %d cents to server\r\n",(trans->payed/SCP_PAYED_THRESHOLD)*SCP_PAYED_THRESHOLD);
					Please_wait_screen(true);
					PT_SCP_MSG_RESERVERINGBETAALD(&Transaction_pt,&SCP_packet,g_Cur_ResvNr,(trans->payed/SCP_PAYED_THRESHOLD)*SCP_PAYED_THRESHOLD);
					send_to_server = ((trans->payed/SCP_PAYED_THRESHOLD)*SCP_PAYED_THRESHOLD);
					trans->payed -= send_to_server;
					trans->payed_SCP += send_to_server;
					if(!(SCP_packet->Data.Message_type == msg_Ok)){
						printf("Error send %d to buffer\r\n",send_to_server);
						BUFFER_SCP_MSG_RESERVERINGBETAALD(g_Cur_ResvNr,send_to_server);
					}
					Please_wait_screen(false);
					SCR_pay(trans,true);
				}
			}
#endif
			Please_wait_screen(false);
			if(Devices_enabled == false){
				PT_SPAWN(&Transaction_pt , &pay_pt, Enable_devices(trans, ev, data));
			}
		}
		PT_YIELD(&Transaction_pt);
	}

	//--------------------------------------------return money--------------------------------------------
	return_label:
	ev = PROCESS_EVENT_INIT;
	while(ev != PROCESS_EVENT_EXIT){
		if(ev == PROCESS_EVENT_INIT){
			if(trans->Device_available.Coin_changer_available == false){
				break;
			}

			if(trans->to_return <= 0){
				break;
			}
			SCR_Return();
			SET_SCREEN_TIMEOUT(4);
			etimer_set(&timeout_tm,CLOCK_SECOND*60);
			printf("pay: Return %d cents\r\n",trans->to_return);
			if(MDB_Betaal(trans->to_return) == false){
				MDB_Betaal(1000);
			}
		}

		if(ev == event_coin_payed){
			trans->payed -= *((uint16_t *)data);
			trans->cash_payed -= *((uint16_t *)data);
			trans->to_return -= *((uint16_t *)data);;
			g_trans_statistics.Coins_return += *((uint16_t *)data);
			printf("pay: Returnd %d cents\r\n",*((uint16_t *)data));
			*((uint16_t *)data) = 0;
			Transaction_statistics_save();
			if(MDB_Warning.PayoutFail == true){
				PT_MESSAGE_SCREEN(&Transaction_pt,GET_TEXT(S_change_failed));
				break;
			}else if(trans->to_return > 0){
				if(MDB_Betaal(trans->to_return) == false){
					MDB_Betaal(1000);
				}
			}
			if(trans->to_return <= 0){
				break;
			}
		}

		if(trans->to_return <= 0){
			if(PROCESS_EVENT_SCREEN_TIMEOUT){
				break;
			}
		}

		if(ev == PROCESS_EVENT_TIMER){
			if(etimer_expired(&timeout_tm)){
				PT_MESSAGE_SCREEN(&Transaction_pt,GET_TEXT(S_change_failed));
				break;
			}
		}

		PT_YIELD(&Transaction_pt);
	}


	if(trans->payed == 0){
		if(trans->Device_available.Printer_available && (trans->payed_SCP > 0)){
			Receipt_transaction_canceled(trans);
		}
		trans->payment_success = false;
		Productlist_clear();
		PT_EXIT(&Transaction_pt);//no server communication needed
	}
	//--------------------------------------------delivery--------------------------------------------

	if(trans->payed >= trans->price){
		PT_SPAWN(&Transaction_pt, &Transaction_sub_pt,Transaction_Delivery_box(&Transaction_sub_pt,&trans->payment_success, ev, data));
	}
	if((trans->payment_success == true) && (trans->Device_available.Coin_acceptor_available == true)){
		EMP_800_reset_value();
	}

	if((trans->payment_success == false)&&(trans->to_return == 0)){
		trans->to_return = trans->payed;
		goto return_label;
	}

	//--------------------------------------------print receipt--------------------------------------------
	Print_receipt_label:
	if(trans->print_receipt == true){
		SCR_printing_receipt();
		if(trans->payment_success){
			Receipt_transaction_accept(trans);
		}else{
			Receipt_transaction_canceled(trans);
		}
		PT_DELAY(&Transaction_pt,CLOCK_SECOND * 3);
	}
	Productlist_clear();
	BUFFER_SCP_MSG_TRANSACTION_DONE(trans->multipass_payed, trans->cash_payed, trans->bank_payed);
	BUFFER_SCP_MSG_CASHSTATUS(send_CashStatus());
	Please_wait_screen(false);
	PT_END(&Transaction_pt);
}

/*
 * @brief draw pay screen
 */
void SCR_pay(Transaction_info_t * trans, bool refresh_all){
		Point_t point = {.x=120,.y=30};

		trans->to_pay = (trans->price - (trans->payed + trans->payed_SCP));

		switch(g_screen_type){
			case SCR_EDIP:

			if(refresh_all == true){
				Edip_Clear_Display();

				Edip_Set_Font(&StdFont);
				point.x = 120;
				point.y = 0;
				Edip_Draw_String(point,GET_TEXT(S_to_pay));
			}
			Edip_Set_Font(&H1Font);
			point.x = 120;
			point.y = 26;
			Edip_Draw_String(point," %c%.2f ",C_EURO,Centen_to_Euro(trans->to_pay));
			if(refresh_all == true){
				Edip_Set_Font(&StdFont);
				if(trans->Device_available.Coin_changer_available || trans->Device_available.Coin_acceptor_available){
					if(MDB_Warning.TubeAlmostEmpty == true || trans->Device_available.Coin_acceptor_available){
						point. x = 10;
						point. y = 60;
						Edip_Draw_String(point,GET_TEXT(S_Empty));
					}
				}

				Edip_Set_Font(&SmallFont);
				Point_t Line_1 = {.x=0,.y=93};
				Point_t Line_2 = {.x=240,.y=93};
				Edip_Draw_Line(Line_1, Line_2);

				point. x = 10;
				point. y = 96;
				if(trans->Device_available.Coin_changer_available || trans->Device_available.Coin_acceptor_available){
					Edip_Draw_String(point,"-%s",GET_TEXT(S_Coins));
					point. x += 62;
					if(ini_getbool("Coin accept","5cent",false,"Accept 5 cent:",inifile_machine)){
						Edip_Draw_String(point,"0,05");
						point. x += 32;
					}
					if(ini_getbool("Coin accept","10cent",true,"Accept 10 cent:",inifile_machine)){
						Edip_Draw_String(point,"0,10");
						point. x += 32;
					}
					if(ini_getbool("Coin accept","20cent",true,"Accept 20 cent:",inifile_machine)){
						Edip_Draw_String(point,"0,20");
						point. x += 32;
					}
					if(ini_getbool("Coin accept","50cent",true,"Accept 50 cent:",inifile_machine)){
						Edip_Draw_String(point,"0,50");
						point. x += 32;
					}
					if(ini_getbool("Coin accept","1euro",true,"Accept 1 euro:",inifile_machine)){
						Edip_Draw_String(point,"1,-");
						point. x += 25;
					}
					if(ini_getbool("Coin accept","2euro",true,"Accept 2 euro:",inifile_machine)){
						Edip_Draw_String(point,"2,-");
					}
					point. x = 10;
					point. y += 10;
				}

				if(trans->Device_available.Cash_available){
					Edip_Draw_String(point,"-%s",GET_TEXT(S_Cash));
					point. x += 60;
					if(g_bill_accept[0]){
						Edip_Draw_String(point,"5,-");
						point. x += 32;
					}
					if(g_bill_accept[1]){
						Edip_Draw_String(point,"10,-");
						point. x += 32;
					}
					if(g_bill_accept[2]){
						Edip_Draw_String(point,"20,-");
						point. x += 32;
					}
					if(g_bill_accept[3]){
						Edip_Draw_String(point,"50,-");
					}
					point. x = 10;
					point. y += 10;
				}

				if(trans->Device_available.Payter_available){

					uint32_t tranaction_cost = RTP_transaction_cost(trans->to_pay);
					if(tranaction_cost == 0){
						Edip_Draw_String(point,"-%s",GET_TEXT(S_Debit_card));
					}else{
						Edip_Draw_String(point,"-%s +%c%.2f",GET_TEXT(S_Debit_card),C_EURO,Centen_to_Euro(tranaction_cost));
					}

					point. y += 10;
				}

				if(trans->Device_available.Multipass_available){
					if(trans->Device_available.QR_available){
						Edip_Draw_String(point,"-%s/%s",GET_TEXT(S_Multipass),GET_TEXT(S_QR_code));
					}else{
						Edip_Draw_String(point,"-%s",GET_TEXT(S_Multipass));
					}
				}
			}
			break;
		default:
			break;
		}
		return;
}

/*
 * @brief draw return screen
 */
void SCR_Return(void){
	Point_t point = {.x=120,.y=30};

	Edip_Clear_Display();

	Edip_Set_Font(&StdFont);
	point.x = 120;
	point.y = 30;
	Edip_Draw_String(point,GET_TEXT(S_Dont_forget_your_change));

	return;
}

/*
 * @brief draw user payment method selection
 */
void SCR_Choose_payment_method(void){
	Point_t point = {.x=120,.y=30};
	Point_t Line_1 = {.x=0,.y=0};
	Point_t Line_2 = {.x=240,.y=20};
	TG2460H_Paper_status_t Paper_status;
	switch(g_screen_type){
	case SCR_EDIP:

		Paper_status = TG2460H_Get_paper_status();
		Edip_Clear_Display();

		Edip_Draw_Fill_Rectangle(Line_1, Line_2);

		Edip_Change_Font_Setting(&StdFont,MODE,M_INVERSE);
		Edip_Set_Font(&StdFont);
		point.x = 120;
		point.y = 5;
		Edip_Draw_String(point,GET_TEXT(S_Choose_payment_method));
		Edip_Change_Font_Setting(&StdFont,MODE,M_REPLACE);
		Edip_Set_Font(&StdFont);
		point.y = 30;
		point.x = 10;
		Edip_Draw_String(point,"1:");
		point.x = 120;
		if(!CCV_startup_done() || (Paper_status == S_DEVICE_UNAVAILABLE)){
			Edip_Draw_String(point,"%s (%s)",GET_TEXT(S_Debit_card), GET_TEXT(S_Out_of_Order));
		}else if((CCV_Get_OverallResult() == R_PRINTLASTTICKET) || (CCV_Get_OverallResult() == R_PRINTER_LOW) || Paper_status == S_PAPER_EMPTY){
			Edip_Draw_String(point,"%s (%s)",GET_TEXT(S_Debit_card), GET_TEXT(S_Needs_service));
		}else{
			Edip_Draw_String(point,GET_TEXT(S_Debit_card));
		}
		Line_1.y = point.y + 20;
		Line_2.y = point.y + 20;
		Edip_Draw_Line(Line_1, Line_2);
		point.x = 10;
		point.y += 25;
		Edip_Draw_String(point,"2:");
		point.x = 120;
		Edip_Draw_String(point,"%s / %s",GET_TEXT(S_Coins),GET_TEXT(S_Cash));
		Line_1.y = point.y + 20;
		Line_2.y = point.y + 20;
		Edip_Draw_Line(Line_1, Line_2);
		break;
	case SCR_PC:
		printf("SCR_PC:[Menu header] %s\r\n",GET_TEXT(S_Choose_payment_method));
		printf("SCR_PC:[Menu 1] %s\r\n",GET_TEXT(S_Debit_card));
		printf("SCR_PC:[Menu 2] %s / %s\r\n",GET_TEXT(S_Coins),GET_TEXT(S_Cash));
		break;
	default:
		break;
	}

}

/*
 * @brief send Chash status to server
 */
SCP_Data_t send_CashStatus(void){
	SCP_Data_t data;
	uint8_t i;

	data.Message_type = msg_CashStatus;
	for(i = 0; i < 6; i++){
		data.UID[i] = CoinTube[i];
	}
	data.UID[6] = (g_trans_statistics.Cash_payed / 500);
	data.Value[0] =	g_trans_statistics.Coins_in_cash_box;
	data.Value[1] = g_trans_statistics.Debit_card_payed;
	return data;
}

/*
 * @brief check cash exchanger level
 */
bool check_cash_level(Transaction_info_t * trans, uint16_t centen){

	uint8_t i;
	uint32_t level = 0;
	int32_t change_level;
	if(trans->Device_available.Coin_changer_available == false){return true;}
	if(SetupDoneCC == false){return false;}

	switch(centen){
	case 500:
		change_level = g_min_change_level[0];
		break;
	case 1000:
		change_level = g_min_change_level[1];
		break;
	case 2000:
		change_level = g_min_change_level[2];
		break;
	case 5000:
		change_level = g_min_change_level[3];
		break;
	default:
		return false;
		break;
	}

	for(i = 0; i < 7; i++){
		level +=  (CoinValue[i] * 5) * CoinTube[i];
		if(level > change_level){
			return true;
		}
	}
	return false;
}

/*
 * @ brief check if bill value is enabled
 */
bool check_bill_accept(uint16_t centen){


	switch(centen){
	case 500:
		if(g_bill_accept[0] == false){return false;}
		break;
	case 1000:
		if(g_bill_accept[1] == false){return false;}
		break;
	case 2000:
		if(g_bill_accept[2] == false){return false;}
		break;
	case 5000:
		if(g_bill_accept[3] == false){return false;}
		break;
	default:
		return false;
		break;
	}
	return true;
}

#endif
