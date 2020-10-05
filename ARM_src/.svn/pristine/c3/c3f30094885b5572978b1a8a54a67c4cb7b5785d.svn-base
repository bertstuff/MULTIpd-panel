#include <Board/Def_config.h>
#if ((PAYING_MODULE == 1) && (RESERVATION_MODULE == 1))

#include <Core/Clock/rtc_cal.h>
#include <Core/Clock/Time_functions.h>
#include <Core/Extensions/ini/minIni.h>
#include <Core/protocol/Net/SCP/SCP_Comm.h>
#include <Core/protocol/Net/SCP/SCP_msg.h>
#include <Core/protocol/Net/SCP/tRFIDmsg.h>
#include <Core/Process/process.h>
#include <Core/Thread/pt.h>
#include <Core/Type/type.h>
#include  <Device/I2C/BUTTONS/Keypad_Parser.h>
#include <Device/I2C/Edip_240B/EDIP_240B_Display.h>
#include <Device/I2C/Edip_240B/EDIP_240B_Draw.h>
#include <Device/I2C/Edip_240B/EDIP_240B_Text.h>
#include <Device/I2C/Edip_240B/EDIP_240B_Types.h>
#include <stdint.h>
#include <stdio.h>
#include <Screen/SCR_Text.h>
#include <Screen/SCR_Init.h>
#include <Processes/Global_UID.h>
#include <Processes/Reservation.h>
#include <Screen/Transaction/SCR_Transaction.h>
#include <time.h>
#include <Core/Timer/timer_functions.h>
#include <Device/I2C/LV3095/LV3095.h>
#include <stdbool.h>
#include <string.h>
#include <Screen/SCR_Message.h>
#include <Screen/Transaction/SCR_Transaction_products.h>
#include <Screen/Transaction/SCR_Reservation_availability.h>
#include <Device/I2C/RFID/RFID.h>
#include <Core/Extensions/ini/minGlue-FatFs.h>


void SCR_Choose_reservation(void);
void SCR_reservation_present(Transaction_info_t * trans);

extern struct pt Transaction_sub_pt;
extern struct pt pay_pt;

/*
 * @brief let the user select a payment method
 */
PT_THREAD(Choose_reservation(Transaction_info_t * trans, process_event_t ev, process_data_t data)){
	PT_BEGIN(&resv_pt);
	SCR_Choose_reservation();
	SET_SCREEN_TIMEOUT(20);
	if(ini_getbool("General","Ask_reservation",1,"Ask for reservation",inifile_machine) == false){
		trans->reservation = RESERVATION_NO;
		trans->Transaction_type = PAY_LOCAL;
		PT_EXIT(&resv_pt);
	}
	while(ev != PROCESS_EVENT_EXIT){
		//--keypad--
		if(PROCESS_EVENT_SCREEN_TIMEOUT){
			trans->reservation = RESERVATION_CANCELD;
			PT_EXIT(&resv_pt);
		}

		if(keypad_pressed(ev)){
			uint8_t key = *((uint8_t *)data);
			if(key == C_RETURN){
				trans->reservation = RESERVATION_CANCELD;
				PT_EXIT(&resv_pt);
			}else if(key == 1){
				trans->reservation = RESERVATION_YES;
				PT_EXIT(&resv_pt);
			}else if(key == 2){
				trans->reservation = RESERVATION_NO;
				trans->Transaction_type = PAY_LOCAL;
				PT_EXIT(&resv_pt);
			}
		}
		PT_YIELD(&resv_pt);
	}
	PT_END(&resv_pt);
}


PT_THREAD(reservation_transaction(Transaction_info_t * trans, process_event_t ev, process_data_t data)){
	SCP_pack_t * SCP_packet;
	int32_t price;
	Reservation_time_t resv_time;

	PT_BEGIN(&Transaction_sub_pt);
	if(trans->Transaction_type == PAY_RESERVATION_ASK){
		printf("Transaction: choose reservation\r\n");
		PT_SPAWN(&Transaction_sub_pt,&resv_pt,Choose_reservation(trans, ev, data));
	}else if((trans->Transaction_type == PAY_RESERVATION)||(trans->Transaction_type == PAY_KNOWN_RESERVATION)){
		trans->reservation = RESERVATION_YES;
	}else{
		trans->reservation = RESERVATION_NO;
	}

	if(trans->reservation == RESERVATION_CANCELD){
		PT_EXIT(&Transaction_sub_pt);
	}


	if(trans->reservation == RESERVATION_YES){
		trans->reservation = RESERVATION_CANCELD;

		if(trans->Transaction_type == PAY_KNOWN_RESERVATION){
			goto reservation_get_price;
		}

		SCR_reservation_present(trans);

		if(trans->QR_available){
			QR_enable(true);
		}

		//--get RFID or QR
		while(1){
			//--QR-code--
			if(trans->QR_available){
				if(ev == event_QR_present){
					SET_SCREEN_TIMEOUT(60);
					Please_wait_screen(true);
					QR_User_t QR_user = QR_get_current_user();
					Set_GLobal_UID(QR_user.UID);
					PT_SPAWN(&Transaction_sub_pt,&pay_pt,Disable_devices(trans, ev, data));
					PT_SCP_MSG_PRESENT(&Transaction_sub_pt,&SCP_packet,g_Cur_UID, trans->machine_type);
					if(SCP_packet->Data.Message_type == msg_Akkoord){
						if(SCP_msg_Akkoord__ResvNr(SCP_packet) != 0){
							//reservation is Oke!
							g_Cur_ResvNr = SCP_msg_Akkoord__ResvNr(SCP_packet);
							PT_SPAWN(&Transaction_sub_pt, &resv_pt,get_reservation_time(&resv_time,ev,data));
							trans->min_available = (resv_time.lenght / 60);
							break;
						}else{
							PT_MESSAGE_SCREEN(&Transaction_sub_pt,GET_TEXT(S_Reservation_unknown));
							PT_EXIT(&Transaction_sub_pt);
						}
					}else if(SCP_packet->Data.Message_type == msg_NietAkkoord){
						//reservation not started
						if(SCP_msg_Akkoord__ResvNr(SCP_packet) != 0){
							g_Cur_ResvNr = SCP_msg_Akkoord__ResvNr(SCP_packet);
							if((trans->move_reservation == true)&&(trans->check_availability)){
								//move reservation forward if possible
								PT_SPAWN(&Transaction_sub_pt, &resv_pt, Reservation_availability(&resv_pt, trans, ev, data));
								PT_SPAWN(&Transaction_sub_pt, &resv_pt,get_reservation_time(&resv_time,ev,data));
								if(resv_time.time_to_end > 0){
									if((resv_time.lenght / 60) < trans->min_available){
										//yes it fit
										trans->min_available = (resv_time.lenght / 60);
										break;
									}
								}
							}else{
								PT_SPAWN(&Transaction_sub_pt, &resv_pt,get_reservation_time(&resv_time,ev,data));
							}
							//reservation not on time
							PT_SPAWN(&Transaction_sub_pt, &resv_pt,show_reservation_message(resv_time ,ev,data));
							PT_EXIT(&Transaction_sub_pt);
						}else{
							PT_MESSAGE_SCREEN(&Transaction_sub_pt,GET_TEXT(S_Reservation_unknown));
							PT_EXIT(&Transaction_sub_pt);
						}
					}else if(SCP_packet->Data.Message_type == msg_QR_Betaal){
						Please_wait_screen(false);
						price = SCP_msg_Betaal__Centen(SCP_packet);
						goto reservation_state;
					}else if(SCP_packet->Data.Message_type == msg_Blokkade){
						printf("SCP: QR Blokkade\r\n");
						PT_MESSAGE_SCREEN(&Transaction_sub_pt,GET_TEXT(S_QR_blocked));
						PT_EXIT(&Transaction_sub_pt);
					}else if(SCP_packet->Data.Message_type == msg_Onbekend){
						printf("SCP: QR onbekend\r\n");
						PT_MESSAGE_SCREEN(&Transaction_sub_pt,GET_TEXT(S_Ticket_unknown));
						PT_EXIT(&Transaction_sub_pt);
					}else{
						printf("SCP: Error\r\n");
						PT_MESSAGE_SCREEN(&Transaction_sub_pt,GET_TEXT(S_Read_error));
						QR_enable(true);
					}
					SCR_reservation_present(trans);
				}
			}


			if(trans->Multipass_available){
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
						Set_GLobal_UID(g_RFID_card->card_UID);
						PT_SPAWN(&Transaction_sub_pt,&pay_pt,Disable_devices(trans, ev, data));
						Please_wait_screen(true);
						PT_SCP_MSG_PRESENT(&Transaction_sub_pt,&SCP_packet,g_Cur_UID, trans->machine_type);
						if(SCP_packet->Data.Message_type == msg_Akkoord){
							if(SCP_msg_Akkoord__ResvNr(SCP_packet) != 0){
								//reservation is Oke!
								g_Cur_ResvNr = SCP_msg_Akkoord__ResvNr(SCP_packet);
								PT_SPAWN(&Transaction_sub_pt, &resv_pt,get_reservation_time(&resv_time,ev,data));
								trans->min_available = (resv_time.lenght / 60);
								break;
							}else{
								PT_MESSAGE_SCREEN(&Transaction_sub_pt,GET_TEXT(S_Reservation_unknown));
								PT_EXIT(&Transaction_sub_pt);
							}
						}else if(SCP_packet->Data.Message_type == msg_NietAkkoord){
							//reservation not started
							if(SCP_msg_Akkoord__ResvNr(SCP_packet) != 0){
								g_Cur_ResvNr = SCP_msg_Akkoord__ResvNr(SCP_packet);
								if((trans->move_reservation == true)&&(trans->check_availability)){
									//move reservation forward if possible
									PT_SPAWN(&Transaction_sub_pt, &resv_pt, Reservation_availability(&resv_pt, trans, ev, data));
									PT_SPAWN(&Transaction_sub_pt, &resv_pt,get_reservation_time(&resv_time,ev,data));
									if(resv_time.time_to_end > 0){
										if((resv_time.lenght / 60) < trans->min_available){
											//yes it fit
											trans->min_available = (resv_time.lenght / 60);
											break;
										}
									}
								}else{
									PT_SPAWN(&Transaction_sub_pt, &resv_pt,get_reservation_time(&resv_time,ev,data));
								}
								//reservation not on time
								PT_SPAWN(&Transaction_sub_pt, &resv_pt,show_reservation_message(resv_time ,ev,data));
								PT_EXIT(&Transaction_sub_pt);
							}else{
								PT_MESSAGE_SCREEN(&Transaction_sub_pt,GET_TEXT(S_Reservation_unknown));
								PT_EXIT(&Transaction_sub_pt);
							}
						}else if(SCP_packet->Data.Message_type == msg_RFID_Betaal){
							Please_wait_screen(false);
							price = SCP_msg_Betaal__Centen(SCP_packet);
							goto reservation_state;
						}else if(SCP_packet->Data.Message_type == msg_Blokkade){
							printf("SCP: RFID Blokkade\r\n");
							PT_MESSAGE_SCREEN(&Transaction_sub_pt,GET_TEXT(S_Pass_blocked));
							PT_EXIT(&Transaction_sub_pt);
						}else if(SCP_packet->Data.Message_type == msg_Onbekend){
							printf("SCP: RFID onbekend\r\n");
							PT_MESSAGE_SCREEN(&Transaction_sub_pt,GET_TEXT(S_Pass_not_valid));
							PT_EXIT(&Transaction_sub_pt);
						}else{
							printf("SCP: Error\r\n");
							PT_MESSAGE_SCREEN(&Transaction_sub_pt,GET_TEXT(S_Read_error));
						}
					}
					QR_enable(true);
					SCR_reservation_present(trans);
				}
			}
			//return
			if(ev == event_Button3x4_pressed){
				uint8_t key = *((uint8_t *)data);
				if(key == C_RETURN){
					trans->reservation = RESERVATION_CANCELD;
					QR_enable(false);
					PT_EXIT(&Transaction_sub_pt);
				}
			}

			//timeout
			if(PROCESS_EVENT_SCREEN_TIMEOUT){
				trans->reservation = RESERVATION_CANCELD;
				QR_enable(false);
				PT_EXIT(&Transaction_sub_pt);
			}
			PT_YIELD(&Transaction_sub_pt);
		}

		//--get price
		reservation_get_price:
		PT_SCP_MSG_RESERVERINGPRICE(&Transaction_sub_pt, &SCP_packet, g_Cur_ResvNr);
		Please_wait_screen(false);
		if(SCP_packet->Data.Message_type == msg_ReserveringPrice){
			price = SCP_msg_ReserveringPrice__price(SCP_packet);
			reservation_state:
			if(price <= 0){
				trans->price = 0;
				trans->reservation = RESERVATION_PAYED;
				PT_EXIT(&Transaction_sub_pt);
			}else{
				trans->reservation = RESERVATION_OK;
				trans->price = price;
				trans->to_pay = trans->price;
				PT_EXIT(&Transaction_sub_pt);
			}
		}else{
			trans->reservation = RESERVATION_CANCELD;
			PT_EXIT(&Transaction_sub_pt);
		}
		Please_wait_screen(false);
	}


	PT_END(&Transaction_sub_pt);
}

/*
 * @brief draw user reservation selection
 */
void SCR_Choose_reservation(void){
	Point_t point = {.x=120,.y=30};
	Point_t Line_1 = {.x=0,.y=0};
	Point_t Line_2 = {.x=240,.y=20};

	Edip_Clear_Display();

	Edip_Draw_Fill_Rectangle(Line_1, Line_2);

	Edip_Change_Font_Setting(&StdFont,MODE,M_INVERSE);
	Edip_Set_Font(&StdFont);
	point.x = 120;
	point.y = 5;
	Edip_Draw_String(point,GET_TEXT(S_Do_you_have_a_reservation));

	point.y = 30;
	point.x = 10;
	Edip_Draw_String(point,"1:");
	point.x = 120;
	Edip_Draw_String(point,GET_TEXT(S_Yes));
	Line_1.y = point.y + 20;
	Line_2.y = point.y + 20;
	Edip_Draw_Line(Line_1, Line_2);
	point.x = 10;
	point.y += 25;
	Edip_Draw_String(point,"2:");
	point.x = 120;
	Edip_Draw_String(point,GET_TEXT(S_No));
	Line_1.y = point.y + 20;
	Line_2.y = point.y + 20;
	Edip_Draw_Line(Line_1, Line_2);

}

void SCR_reservation_present(Transaction_info_t * trans){
	Point_t point = {.x=120,.y=30};

	Edip_Clear_Display();
	Edip_Set_Font(&StdFont);
	point.x = 120;
	point.y = 30;
	if(trans->Multipass_available){
		if(trans->QR_available){
			Edip_Draw_String(point,GET_TEXT(S_Scan_Multipass_QR));
		}else{
			Edip_Draw_String(point,GET_TEXT(S_Scan_Multipass));
		}
	}
	return;
}

#endif
