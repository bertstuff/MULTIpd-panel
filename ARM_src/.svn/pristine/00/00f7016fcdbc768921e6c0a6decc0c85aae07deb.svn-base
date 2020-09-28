#include <Board/Def_config.h>
#if(MENU_MODULE == 1)

/*Include*/
#include <Core/Clock/rtc_cal.h>
#include <Core/Process/process.h>
#include <Core/Thread/pt.h>
#include <Core/Type/type.h>
#include <Device/I2C/BUTTONS/Keypad_Parser.h>
#include <Device/I2C/Edip_240B/EDIP_240B_Display.h>
#include <Device/I2C/Edip_240B/EDIP_240B_Draw.h>
#include <Device/I2C/Edip_240B/EDIP_240B_Text.h>
#include <Device/UART/MDB-C21/MDB-ARB-C21-7TUBES.h>
#include <Device/Net/CCV/CCV.h>
#include <Global.h>
#include <stdint.h>
#include <Screen/Menu/SCR_Menu_func.h>
#include <Screen/Menu/SCR_Menu_handler.h>
#include <Screen/SCR_Init.h>
#include <Screen/SCR_Text.h>
#include <Screen/Transaction/SCR_Transaction_Receipt.h>
#include <Screen/Transaction/SCR_Transaction_statistics.h>
#include <Screen/Transaction/SCR_Transaction_init.h>
#include <Core/Util/math_util.h>
#include <processes/IO_process.h>
#include "GLobal.h"

/*prototypes*/

void SCR_statistics(bool enter_menu);
PT_THREAD(SCR_statistics_handler(process_event_t ev, process_data_t data));

/*global var*/
extern int32_t select_val;

//--------------------------------------------exit handler------------------------------------------------------------//
void SCR_daily_cash_register_handler(menu_handler_t event){
	Transaction_info_t trans;
	if(event == H_MENU_ACCEPT){
		Transaction_get_devices(&trans);
		Receipt_daily_cash_register_closure(&trans);
		CCV_increase_ShiftNumber();
		g_trans_statistics.Cash_payed = 0;
		g_trans_statistics.Coins_in_cash_box = 0;
		g_trans_statistics.Coins_payed = 0;
		g_trans_statistics.Coins_return = 0;
		g_trans_statistics.Debit_card_payed = 0;
		g_trans_statistics.Multipass_payed = 0;
		g_trans_statistics.Reservations = 0;
		g_trans_statistics.Transactions = 0;
		rtc_cal_gettime(&g_trans_statistics.Last_reset);
		#ifdef O_CABINET_LOCK
			IO_set_output_state(O_CABINET_LOCK, true);
		#endif
		Transaction_statistics_save();
		menu_set_exit_message(cur_menu,REF_TEXT(S_Reset_statistics));
	}else{
		menu_set_exit_message(cur_menu,NULL);
	}
	return;
}

//--------------------------------------------Menu------------------------------------------------------------//
void SCR_Revenue_Menu(struct Menu_t * Menu){
	struct Menu_t * Menu_Revenue;
	struct Menu_t * Menu_Statistics, * Menu_daily_cash_register;
	struct Menu_item_t * Item_Statistics, * Item_daily_cash_register, * Item_Revenue;

	Item_Revenue = menu_add_menu_item(Menu,REF_TEXT(S_Revenue));

	Menu_Revenue = menu_new_menu(REF_TEXT(S_Revenue), Item_Revenue);
	Item_Statistics = menu_add_menu_item(Menu_Revenue,REF_TEXT(S_Statistics));
	Item_daily_cash_register = menu_add_menu_item(Menu_Revenue,REF_TEXT(S_daily_cash_register_closure));

	Menu_Statistics = menu_new_custom_menu(REF_TEXT(S_Statistics), SCR_statistics_handler, SCR_statistics, Item_Statistics);
	menu_set_menu_auth(Menu_Statistics,AUTH_ADMIN | AUTH_MANUFACTURE | AUTH_DEBUG);

	Menu_daily_cash_register = menu_new_question_menu(REF_TEXT(S_daily_cash_register_closure), REF_TEXT(S_Are_you_sure_to_reset_the_statistics) ,Item_daily_cash_register);
	menu_set_menu_function(Menu_daily_cash_register, SCR_daily_cash_register_handler);
	menu_set_menu_auth(Menu_daily_cash_register,AUTH_ADMIN | AUTH_MANUFACTURE | AUTH_DEBUG);
	return;
}

PT_THREAD(SCR_statistics_handler(process_event_t ev, process_data_t data)){
	PT_BEGIN(&menu_pt);
	if(keypad_pressed(ev)){
		uint8_t button= *((uint8_t *)data);
		if(button == C_RETURN){
			SCR_leave_menu(H_MENU_CANCEL);
			PT_EXIT(&menu_pt);
		}
	}
	PT_END(&menu_pt);
}

void SCR_statistics(bool enter_menu){
	Point_t point = {.x=120,.y=5};
	float Calc_price;
	Transaction_info_t trans;

	Transaction_get_devices(&trans);

	Edip_Set_Font(&StdFont);
	Edip_Clear_Display();
	Edip_Draw_String(point,"%s: %d",GET_TEXT(S_Transactions), g_trans_statistics.Transactions);
	if(trans.Coin_acceptor_available || trans.Coin_changer_available){
		point.y += 15;
		Edip_Draw_String(point,"%s: %c%.2f",GET_TEXT(S_Coins_payed),C_EURO ,Centen_to_Euro(g_trans_statistics.Coins_payed));
		if(trans.Coin_changer_available){
			point.y += 15;
			Edip_Draw_String(point,"%s: %c%.2f",GET_TEXT(S_Coins_returned),C_EURO ,Centen_to_Euro(g_trans_statistics.Coins_return));
			point.y += 15;
			Edip_Draw_String(point,"%s: %c%.2f",GET_TEXT(S_Coins_in_box),C_EURO ,Centen_to_Euro(g_trans_statistics.Coins_in_cash_box * CoinSF));
		}
	}
	if(trans.Cash_available){
		point.y += 15;
		Edip_Draw_String(point,"%s: %c%.2f",GET_TEXT(S_Cash_payed),C_EURO ,Centen_to_Euro(g_trans_statistics.Cash_payed));
	}
	if(trans.Payter_available || trans.ATM_available){
		point.y += 15;
		Edip_Draw_String(point,"%s: %c%.2f",GET_TEXT(S_Debit_card_payed),C_EURO ,Centen_to_Euro(g_trans_statistics.Debit_card_payed));
	}
	if(trans.Multipass_available){
		point.y += 15;
		Edip_Draw_String(point,"%s: %c%.2f",GET_TEXT(S_Multipass_payed),C_EURO ,Centen_to_Euro(g_trans_statistics.Multipass_payed));
	}
	point.y += 15;
	Calc_price = g_trans_statistics.Coins_payed + g_trans_statistics.Cash_payed;
	Calc_price -= g_trans_statistics.Coins_return;
	Calc_price += g_trans_statistics.Debit_card_payed + g_trans_statistics.Multipass_payed;
	Calc_price /= 100;
	Edip_Draw_String(point,"%s: %c%.2f",GET_TEXT(S_Total_payed),C_EURO ,Calc_price);

}

#endif
