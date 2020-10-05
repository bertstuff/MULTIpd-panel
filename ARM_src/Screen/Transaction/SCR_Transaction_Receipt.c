#include <Board/Def_config.h>
#if (PAYING_MODULE == 1)
#include <Core/Extensions/ini/minIni.h>
#include <Core/Memory/list.h>
#include <Core/Util/math_util.h>
#ifdef USE_VK80_RECEIPT
#include <Device/UART/VK80/VK80.h>
#else
#include <Device/UART/TG2460H/TG2460H.h>
#endif
#include <Screen/Transaction/SCR_Transaction_products.h>
#include <Screen/Transaction/SCR_Transaction_ATM.h>
#include <Screen/Transaction/SCR_Transaction_statistics.h>
#include <time.h>
#include <Core/Clock/rtc_cal.h>
#include <Core/Clock/Time_functions.h>
#include <Device/UART/MDB-C21/MDB-ARB-C21-7TUBES.h>
#include <Device/Net/CCV/CCV.h>
#include <Processes/Global_UID.h>
#include  <Device/I2C/BUTTONS/Keypad_Parser.h>
#include <Core/Timer/timer_functions.h>

extern struct pt Transaction_sub_pt;

bool Trans_state_change(Transaction_state_t state) __attribute__((weak));

void Receipt_init(void){
#ifndef USE_VK80_RECEIPT
	//Load logo for receipt
	char Setting_Buffer[60];
	ini_gets("Receipt header","Logo","SD1:/IMAGE/LOGO/logo.bmp","Receipt logo:",Setting_Buffer,60,inifile_machine);
	if(strlen(Setting_Buffer) > 1){
		TG2460H_Download_BMP_Logo(0,Setting_Buffer);
	}
#endif
}

void Receipt_time(void){
#ifdef USE_VK80_RECEIPT
	struct tm Cur_time;
	struct tm * local_time;

	VK80_Font_t Font;

	Font.F_bold = false;
	Font.F_double_strike = false;
	Font.F_underline = T_0PX;
	Font.Height = C_NORMAL;
	Font.Width = C_NORMAL;
	Font.cpi = CPI_FONTA_20;
	Font.Font = 'A';
	VK80_Set_font(&Font);
	VK80_Set_font(&Font);

	rtc_cal_gettime(&Cur_time);
	local_time = LocalTime(&Cur_time);
	VK80_Print_string("%d-%d-%d  ", local_time->tm_mday, local_time->tm_mon + 1, local_time->tm_year + 1900);
	VK80_Print_string("%02d:%02d\r\n", local_time->tm_hour, local_time->tm_min);
#else
	struct tm Cur_time;
	struct tm * local_time;

	TG2460H_Font_t Font;
	Font.F_bold = false;
	Font.F_double_strike = false;
	Font.F_underline = T_0PX;
	Font.Height = C_NORMAL;
	Font.Width = C_NORMAL;
	Font.cpi = CPI_FONTB_15;
	Font.Font = FONTB;
	TG2460H_Set_font(&Font);

	rtc_cal_gettime(&Cur_time);
	local_time = LocalTime(&Cur_time);
	TG2460H_Print_string("%d-%d-%d  ", local_time->tm_mday, local_time->tm_mon + 1, local_time->tm_year + 1900);
	TG2460H_Print_string("%02d:%02d\r\n", local_time->tm_hour, local_time->tm_min);
#endif
}

void Receipt_UID(void){
#ifdef USE_VK80_RECEIPT
	VK80_Font_t Font;

	Font.F_bold = false;
	Font.F_double_strike = false;
	Font.F_underline = T_0PX;
	Font.Height = C_NORMAL;
	Font.Width = C_NORMAL;
	Font.cpi = CPI_FONTA_20;
	Font.Font = 'A';
	VK80_Set_font(&Font);
	VK80_Set_font(&Font);
	VK80_Print_string("\nUID:%02X-%02X-%02X-%02X-%02X-%02X-%02X\r\n\n",g_Cur_UID.UID[0],g_Cur_UID.UID[1],g_Cur_UID.UID[2],g_Cur_UID.UID[3],g_Cur_UID.UID[4],g_Cur_UID.UID[5],g_Cur_UID.UID[6]);
#else
	TG2460H_Font_t Font;
	Font.F_bold = false;
	Font.F_double_strike = false;
	Font.F_underline = T_0PX;
	Font.Height = C_NORMAL;
	Font.Width = C_NORMAL;
	Font.cpi = CPI_FONTB_20;
	Font.Font = FONTB;
	TG2460H_Set_font(&Font);

	TG2460H_Print_string("\nUID:%02X-%02X-%02X-%02X-%02X-%02X-%02X\r\n\n",g_Cur_UID.UID[0],g_Cur_UID.UID[1],g_Cur_UID.UID[2],g_Cur_UID.UID[3],g_Cur_UID.UID[4],g_Cur_UID.UID[5],g_Cur_UID.UID[6]);
#endif
}

void Receipt_header(void){
#ifdef USE_VK80_RECEIPT
#else
	char Setting_Buffer[60];
	TG2460H_Font_t Font;

	Font.F_bold = false;
	Font.F_double_strike = false;
	Font.F_underline = T_0PX;
	Font.Height = C_NORMAL;
	Font.Width = C_NORMAL;
	Font.cpi = CPI_FONTB_15;
	Font.Font = FONTB;

	//print Receipt header Lines
	ini_gets("Receipt header","Logo","SD1:/IMAGE/LOGO/logo.bmp","Receipt logo:",Setting_Buffer,60,inifile_machine);
	if(strlen(Setting_Buffer) > 1){
		TG2460H_Print_Logo(0);
	}

	TG2460H_Set_font(&Font);
	TG2460H_Print_string("%s %s\r\n",_ALIGN_C_,_BOLD_ON_);
	ini_gets("Receipt header","Name","Name","Name of company:", Setting_Buffer, 30, inifile_machine);
	TG2460H_Print_string("%s\r\n",Setting_Buffer);
	ini_gets("Receipt header","Address","Address","Address:", Setting_Buffer, 30, inifile_machine);
	TG2460H_Print_string("%s\r\n",Setting_Buffer);
	ini_gets("Receipt header","Port_number","","Port_number:", Setting_Buffer, 30, inifile_machine);
	if(Setting_Buffer[0] != '\0'){
		TG2460H_Print_string("%s %s\r\n",GET_TEXT(S_Port_number),Setting_Buffer);
	}
	ini_gets("Receipt header","Zip_code","Zip_code","Zip code:", Setting_Buffer, 30, inifile_machine);
	TG2460H_Print_string("%s ",Setting_Buffer);
	ini_gets("Receipt header","City","City","City:", Setting_Buffer, 30, inifile_machine);
	TG2460H_Print_string("%s\r\n",Setting_Buffer);
	ini_gets("Receipt header","Tel","Tel","Telephone number:", Setting_Buffer, 30, inifile_machine);
	TG2460H_Print_string("%s\r\n",Setting_Buffer);

	Font.F_bold = false;
	Font.F_double_strike = false;
	Font.F_underline = T_0PX;
	Font.Height = C_NORMAL;
	Font.Width = C_NORMAL;
	Font.cpi = CPI_FONTB_20;
	Font.Font = FONTB;
	TG2460H_Set_font(&Font);

	ini_gets("Receipt header","Text","S_Receipt_header","Extra header text:", Setting_Buffer, 30, inifile_machine);
	if(strlen(Setting_Buffer)){
		TG2460H_Print_string("\r\n%s\r\n\n",ini_text(Setting_Buffer,g_Language, g_screen_type));
	}

	TG2460H_Print_string("%s",_BOLD_OFF_);
#endif
}


void Receipt_Sales_lines(void){
#ifdef USE_VK80_RECEIPT
#else
	int32_t price;
	uint8_t BTW;
	//print Receipt Sales Lines
	TG2460H_Font_t Font;
	Font.F_bold = false;
	Font.F_double_strike = false;
	Font.F_underline = T_0PX;
	Font.Height = C_NORMAL;
	Font.Width = C_NORMAL;
	Font.cpi = CPI_FONTB_20;
	Font.Font = FONTB;
	TG2460H_Set_font(&Font);


	LIST(Product_list);

	Productlist_Handle(Product_list);
	struct Product_t * Product;
	Product = list_head(Product_list);
	while(1){
		if(Product == NULL){break;}
		TG2460H_absolute_position(0);
		if(Product->Product_string == NULL){
				TG2460H_Print_string("%s:",GET_TEXT(Product->Product_name));
			}else{
				TG2460H_Print_string("%s:",Product->Product_string);
			}
		TG2460H_absolute_position(1.6);
		TG2460H_Print_string("%.2f\r\n",Centen_to_Euro(Product->Price));
		Product = list_item_next(Product);
	}
	BTW = ini_getl("Receipt header","BTW_prec",0,"BTW percentage:",inifile_machine);
	if(BTW != 0){
		TG2460H_absolute_position(0);
		TG2460H_Print_string("--------------------------------\r\n");
		TG2460H_Print_string("%s",GET_TEXT(S_EXCL_BTW));
		TG2460H_absolute_position(1.3);
		if(Productlist_total_price(&price)){
			TG2460H_Print_string("EURO %.2f\r\n",Centen_to_Euro((price*100)/(BTW+100)));
		}
		TG2460H_absolute_position(0);
		TG2460H_Print_string("%d%% %s",BTW,GET_TEXT(S_BTW));
		TG2460H_absolute_position(1.3);
		if(Productlist_total_price(&price)){
			TG2460H_Print_string("EURO %.2f\r\n",Centen_to_Euro((price*19)/(BTW+100)));
		}

	}
	TG2460H_absolute_position(0);
	TG2460H_Print_string("--------------------------------\r\n");
	TG2460H_Print_string("%s",_BOLD_ON_);
	TG2460H_Print_string("%s",GET_TEXT(S_Total));
	TG2460H_absolute_position(1.3);
	if(Productlist_total_price(&price)){
		TG2460H_Print_string("EURO %.2f\r\n",Centen_to_Euro(price));
	}
	TG2460H_Print_string("%s",_BOLD_OFF_);
#endif
}

void Receipt_footer(void){
#ifdef USE_VK80_RECEIPT
#else
	char Setting_Buffer[25];
	TG2460H_Font_t Font;

	Font.F_bold = false;
	Font.F_double_strike = false;
	Font.F_underline = T_0PX;
	Font.Height = C_NORMAL;
	Font.Width = C_NORMAL;
	Font.cpi = CPI_FONTB_15;
	Font.Font = FONTB;
	TG2460H_Set_font(&Font);
	TG2460H_Set_Font_Size(C_NORMAL,C_NORMAL);
	//print Receipt footer Lines
	ini_gets("Receipt footer","Text","S_Receipt_footer","Extra footer text:", Setting_Buffer, 25, inifile_machine);
	if(strlen(Setting_Buffer)){
		TG2460H_Print_string("%s\r\n",ini_text(Setting_Buffer,g_Language, g_screen_type));
	}
	Receipt_UID();
	TG2460H_Cut_paper();
#endif
}



void Receipt_transaction_canceled(Transaction_info_t * trans){
#ifdef USE_VK80_RECEIPT
#else
	Receipt_header();
	Receipt_Sales_lines();
	TG2460H_absolute_position(0);
	TG2460H_Print_string("%s",_BOLD_ON_);
	TG2460H_Print_string("Betaald:");
	TG2460H_absolute_position(1.4);
	TG2460H_Print_string("EURO %.2f\r\n",Centen_to_Euro(trans->payed + trans->payed_SCP));

	TG2460H_Print_string("Te betalen:");
	TG2460H_absolute_position(1.4);
	TG2460H_Print_string("EURO %.2f\r\n",Centen_to_Euro(trans->price-(trans->payed + trans->payed_SCP)));

	TG2460H_Print_string("%s",_BOLD_OFF_);
	Receipt_time();
	TG2460H_Set_Font_Size(C_DOUBLE,C_DOUBLE);
	TG2460H_Print_string("CANCELED\r\n");
	Receipt_footer();
#endif
}


void Receipt_transaction_accept(Transaction_info_t * trans){
#ifdef USE_VK80_RECEIPT
#else
	Receipt_header();
	Receipt_Sales_lines();
	TG2460H_absolute_position(0);
	TG2460H_Print_string("%s",_BOLD_ON_);
	TG2460H_Print_string("Betaald:");
	TG2460H_absolute_position(1.4);
	TG2460H_Print_string("EURO %.2f\r\n",Centen_to_Euro(trans->payed + trans->payed_SCP));

	TG2460H_Print_string("%s",_BOLD_OFF_);
	Receipt_time();
	TG2460H_Set_Font_Size(C_DOUBLE,C_DOUBLE);
	TG2460H_Print_string("BETAALD\r\n");
	Receipt_footer();
#endif
}

void Receipt_daily_cash_register_closure(Devices_available_t devices_available){
	struct tm * local_time;

#ifdef USE_VK80_RECEIPT
	VK80_Font_t Font;
	uint8_t i;
	int32_t Total = 0;

	Font.F_bold = false;
	Font.F_double_strike = false;
	Font.F_underline = T_0PX;
	Font.Height = C_NORMAL;
	Font.Width = C_NORMAL;
	Font.cpi = CPI_FONTA_20;
	Font.Font = 'A';
	VK80_Set_font(&Font);
	VK80_Set_font(&Font);
	VK80_Print_string("%s %s\r\n",_ALIGN_C_,_BOLD_ON_);
	VK80_Print_string(GET_TEXT(S_daily_cash_register_closure));
	VK80_Print_string("%s\r\n",_BOLD_OFF_);
	VK80_Print_string("\r\nDate & Time:\r\n");
	Receipt_time();

	local_time = LocalTime(&g_trans_statistics.Last_reset);
	VK80_Print_string("\r\nLast closure:\r\n");
	VK80_Print_string("%d-%d-%d  ", local_time->tm_mday, local_time->tm_mon + 1, local_time->tm_year + 1900);
	VK80_Print_string("%02d:%02d\r\n", local_time->tm_hour, local_time->tm_min);
	VK80_Print_string("\r\n");

	VK80_Print_string("%s: %d\r\n",GET_TEXT(S_Transactions),g_trans_statistics.Transactions);
	if(devices_available.Coin_changer_available){
		VK80_Print_string("\r\n");
		VK80_Print_string("%s:\r\n",GET_TEXT(S_Coins_in_machine));

		Total = 0;
		for(i = 0; i < 7; i++){
			if(CoinValue[i] != 0){
				VK80_Print_string("%d CENT: %dX \r\n",(CoinValue[i] * 5),CoinTube[i]);
				Total +=  (CoinValue[i] * 5) * CoinTube[i];
			}
		}
		VK80_Print_string("--------------------------------\r\n");
		VK80_Print_string("%s: EURO %.2f\r\n",GET_TEXT(S_Total),Centen_to_Euro(Total));
		VK80_Print_string("%s: EURO %.2f\r\n",GET_TEXT(S_Coins_in_box),Centen_to_Euro((g_trans_statistics.Coins_in_cash_box * CoinSF)));
		VK80_Print_string("%s: EURO %.2f\r\n",GET_TEXT(S_Coins_thrown),Centen_to_Euro(g_trans_statistics.Coins_payed));
		VK80_Print_string("%s: EURO %.2f\r\n",GET_TEXT(S_Coins_returned),Centen_to_Euro(g_trans_statistics.Coins_return));
	}

	VK80_Print_string("\r\n");

	if((devices_available.Coin_changer_available)||(devices_available.Coin_acceptor_available)){
		VK80_Print_string("%s: EURO %.2f\r\n",GET_TEXT(S_Coins_payed),Centen_to_Euro(g_trans_statistics.Coins_payed - g_trans_statistics.Coins_return));
	}

	if(devices_available.Cash_available){
		VK80_Print_string("%s: EURO %.2f\r\n",GET_TEXT(S_Cash_payed),Centen_to_Euro(g_trans_statistics.Cash_payed));
	}

	if(devices_available.Payter_available || devices_available.ATM_available){
		VK80_Print_string("%s: EURO %.2f\r\n",GET_TEXT(S_Debit_card_payed),Centen_to_Euro(g_trans_statistics.Debit_card_payed));
		if(trans->ATM_available){
			VK80_Print_string("%s: %d\r\n",GET_TEXT(S_ShiftNumber),CCV_get_ShiftNumber());
		}
	}

	if(devices_available.Multipass_available || devices_available.QR_available){
		VK80_Print_string("%s: EURO %.2f\r\n",GET_TEXT(S_Multipass_payed),Centen_to_Euro(g_trans_statistics.Multipass_payed));
	}
#if(RESERVATION_MODULE == 1)
	if(g_trans_statistics.Reservations != 0){
		VK80_Print_string("%s: %d\r\n",GET_TEXT(S_Reservations),g_trans_statistics.Reservations);
	}
#endif
	if(devices_available.Coin_changer_available){
		Total += ((g_trans_statistics.Coins_in_cash_box * CoinSF) + g_trans_statistics.Cash_payed);
		VK80_Print_string("%s: EURO %.2f\r\n",GET_TEXT(S_Money_in_machine),Centen_to_Euro(Total));
	}
	Total = 0;
	Total += g_trans_statistics.Coins_payed;
	Total -= g_trans_statistics.Coins_return;
	Total += g_trans_statistics.Cash_payed;
	Total += g_trans_statistics.Debit_card_payed;
	Total += g_trans_statistics.Multipass_payed;
	VK80_Print_string("%s",_BOLD_ON_);
	VK80_Print_string("%s: EURO %.2f\r\n",GET_TEXT(S_Total_payed),Centen_to_Euro(Total));
	VK80_Print_string("%s",_BOLD_OFF_);

	Receipt_UID();
	VK80_Cut_paper();

#else
	TG2460H_Font_t Font;
	uint8_t i;
	int32_t Total = 0;

	Font.F_bold = false;
	Font.F_double_strike = false;
	Font.F_underline = T_0PX;
	Font.Height = C_NORMAL;
	Font.Width = C_NORMAL;
	Font.cpi = CPI_FONTB_20;
	Font.Font = FONTB;
	TG2460H_Set_font(&Font);

	TG2460H_Print_string("%s",_BOLD_ON_);
	TG2460H_Print_string(GET_TEXT(S_daily_cash_register_closure));
	TG2460H_Print_string("%s\r\n",_BOLD_OFF_);
	Receipt_time();

	local_time = LocalTime(&g_trans_statistics.Last_reset);
	TG2460H_Print_string("\r\nLast closure:\r\n");
	TG2460H_Print_string("%d-%d-%d  ", local_time->tm_mday, local_time->tm_mon + 1, local_time->tm_year + 1900);
	TG2460H_Print_string("%02d:%02d\r\n", local_time->tm_hour, local_time->tm_min);
	TG2460H_Print_string("\r\n");

	TG2460H_Print_string("%s: %d\r\n",GET_TEXT(S_Transactions),g_trans_statistics.Transactions);
	TG2460H_Print_string("\r\n");
	if(devices_available.Coin_changer_available){
		TG2460H_Print_string("%s:\r\n",GET_TEXT(S_Coins_in_machine));

		Total = 0;
		for(i = 0; i < 7; i++){
			if(CoinValue[i] != 0){
				TG2460H_Print_string("%d CENT: %dX \r\n",(CoinValue[i] * 5),CoinTube[i]);
				Total +=  (CoinValue[i] * 5) * CoinTube[i];
			}
		}
		TG2460H_Print_string("--------------------------------\r\n");
		TG2460H_Print_string("%s: EURO %.2f\r\n",GET_TEXT(S_Total),Centen_to_Euro(Total));
		TG2460H_Print_string("%s: EURO %.2f\r\n",GET_TEXT(S_Coins_in_box),Centen_to_Euro((g_trans_statistics.Coins_in_cash_box * CoinSF)));
		TG2460H_Print_string("%s: EURO %.2f\r\n",GET_TEXT(S_Coins_thrown),Centen_to_Euro(g_trans_statistics.Coins_payed));
		TG2460H_Print_string("%s: EURO %.2f\r\n",GET_TEXT(S_Coins_returned),Centen_to_Euro(g_trans_statistics.Coins_return));
	}

	TG2460H_Print_string("\r\n");

	if(devices_available.Coin_changer_available){
		TG2460H_Print_string("%s: EURO %.2f\r\n",GET_TEXT(S_Coins_payed),Centen_to_Euro(g_trans_statistics.Coins_payed - g_trans_statistics.Coins_return));
	}

	if(devices_available.Cash_available){
		TG2460H_Print_string("%s: EURO %.2f\r\n",GET_TEXT(S_Cash_payed),Centen_to_Euro(g_trans_statistics.Cash_payed));
	}

	if(devices_available.Payter_available || devices_available.ATM_available){
		TG2460H_Print_string("%s: EURO %.2f\r\n",GET_TEXT(S_Debit_card_payed),Centen_to_Euro(g_trans_statistics.Debit_card_payed));
		if(devices_available.ATM_available){
			TG2460H_Print_string("%s: %d\r\n",GET_TEXT(S_ShiftNumber),CCV_get_ShiftNumber());
		}
	}

	if(devices_available.Multipass_available || devices_available.QR_available){
		TG2460H_Print_string("%s: EURO %.2f\r\n",GET_TEXT(S_Multipass_payed),Centen_to_Euro(g_trans_statistics.Multipass_payed));
	}
#if(RESERVATION_MODULE == 1)
	if(g_trans_statistics.Reservations != 0){
		TG2460H_Print_string("%s: %d\r\n",GET_TEXT(S_Reservations),g_trans_statistics.Reservations);
	}
#endif

	Total += ((g_trans_statistics.Coins_in_cash_box * CoinSF) + g_trans_statistics.Cash_payed);
	TG2460H_Print_string("%s: EURO %.2f\r\n",GET_TEXT(S_Money_in_machine),Centen_to_Euro(Total));
	Total = 0;
	Total += g_trans_statistics.Coins_payed;
	Total -= g_trans_statistics.Coins_return;
	Total += g_trans_statistics.Cash_payed;
	Total += g_trans_statistics.Debit_card_payed;
	Total += g_trans_statistics.Multipass_payed;
	TG2460H_Print_string("%s",_BOLD_ON_);
	TG2460H_Print_string("%s: EURO %.2f\r\n",GET_TEXT(S_Total_payed),Centen_to_Euro(Total));
	TG2460H_Print_string("%s",_BOLD_OFF_);

	Receipt_UID();
	TG2460H_Cut_paper();
#endif
}


void Receipt_Cabinet_opened(Transaction_info_t * trans){
#ifdef USE_VK80_RECEIPT
#else
	TG2460H_Font_t Font;
	uint8_t i;
	int32_t Total = 0;

	Font.F_bold = false;
	Font.F_double_strike = false;
	Font.F_underline = T_0PX;
	Font.Height = C_NORMAL;
	Font.Width = C_NORMAL;
	Font.cpi = CPI_FONTB_20;
	Font.Font = FONTB;
	TG2460H_Set_font(&Font);

	TG2460H_Print_string("%s",_BOLD_ON_);
	TG2460H_Print_string(GET_TEXT(S_Cabinet_opened));
	TG2460H_Print_string("%s\r\n",_BOLD_OFF_);
	Receipt_time();
	TG2460H_Print_string("\r\n");

	TG2460H_Print_string("%s: %d\r\n",GET_TEXT(S_Transactions),g_trans_statistics.Transactions);
	TG2460H_Print_string("\r\n");
	if(trans->Device_available.Coin_changer_available){
		TG2460H_Print_string("%s:\r\n",GET_TEXT(S_Coins_in_machine));

		Total = 0;
		for(i = 0; i < 7; i++){
			if(CoinValue[i] != 0){
				TG2460H_Print_string("%d CENT: %dX \r\n",(CoinValue[i] * 5),CoinTube[i]);
				Total +=  (CoinValue[i] * 5) * CoinTube[i];
			}
		}
		TG2460H_Print_string("--------------------------------\r\n");
		TG2460H_Print_string("%s: EURO %.2f\r\n",GET_TEXT(S_Total),Centen_to_Euro(Total));
		TG2460H_Print_string("%s: EURO %.2f\r\n",GET_TEXT(S_Coins_in_box),Centen_to_Euro((g_trans_statistics.Coins_in_cash_box * CoinSF)));
	}

	TG2460H_Print_string("\r\n");

	if(trans->Device_available.Cash_available){
		TG2460H_Print_string("%s: EURO %.2f\r\n",GET_TEXT(S_Cash_payed),Centen_to_Euro(g_trans_statistics.Cash_payed));
	}

	TG2460H_Print_string("\r\n");

	Total += ((g_trans_statistics.Coins_in_cash_box * CoinSF) + g_trans_statistics.Cash_payed);
	TG2460H_Print_string("%s: EURO %.2f\r\n",GET_TEXT(S_Money_in_machine),Centen_to_Euro(Total));

	Receipt_UID();
	TG2460H_Cut_paper();
#endif
}

void SCR_receipt_transaction(void){
	Point_t point = {.x=120,.y=30};
	Point_t Line_1 = {.x=0,.y=0};
	Point_t Line_2 = {.x=240,.y=20};

	switch(g_screen_type){
	case SCR_EDIP:
		Edip_Clear_Display();
		Edip_Draw_Fill_Rectangle(Line_1, Line_2);
		Edip_Change_Font_Setting(&StdFont,MODE,M_INVERSE);
		Edip_Set_Font(&StdFont);
		point.x = 120;
		point.y = 5;
		Edip_Draw_String(point,GET_TEXT(S_Do_you_want_a_receipt));
		Edip_Change_Font_Setting(&StdFont,MODE,M_REPLACE);
		Edip_Set_Font(&StdFont);
		point.y = 30;
		point.x = 10;
		Edip_Draw_String(point,"1:");
		point.x = 120;
		Edip_Draw_String(point,GET_TEXT(S_Print_receipt));
		Line_1.y = point.y + 20;
		Line_2.y = point.y + 20;
		Edip_Draw_Line(Line_1, Line_2);
		point.x = 10;
		point.y += 25;
		Edip_Draw_String(point,"2:");
		point.x = 120;
		Edip_Draw_String(point,"%s",GET_TEXT(S_No_receipt));
		Line_1.y = point.y + 20;
		Line_2.y = point.y + 20;
		Edip_Draw_Line(Line_1, Line_2);
		break;
	case SCR_PC:
		printf("SCR_PC:[menu header] %s\r\n",GET_TEXT(S_Do_you_want_a_receipt));
		printf("SCR_PC:[menu 1] %s\r\n",GET_TEXT(S_Print_receipt));
		printf("SCR_PC:[menu 2] %s\r\n",GET_TEXT(S_No_receipt));
		break;
	default:
		break;
	}
}

void SCR_no_paper(void){
	Point_t point = {.x=120,.y=30};
	Point_t Line_1 = {.x=0,.y=0};
	Point_t Line_2 = {.x=240,.y=20};

	switch(g_screen_type){
	case SCR_EDIP:
		Edip_Clear_Display();

		Edip_Draw_Fill_Rectangle(Line_1, Line_2);

		Edip_Change_Font_Setting(&StdFont,MODE,M_INVERSE);
		Edip_Set_Font(&StdFont);
		point.x = 120;
		point.y = 5;
		Edip_Draw_String(point,GET_TEXT(S_Cant_print_receipt));
		Edip_Change_Font_Setting(&StdFont,MODE,M_REPLACE);
		Edip_Set_Font(&StdFont);
		point.y = 30;
		point.x = 10;
		Edip_Draw_String(point,"1:");
		point.x = 120;
		Edip_Draw_String(point,GET_TEXT(S_Continue_without_receipt));
		Line_1.y = point.y + 20;
		Line_2.y = point.y + 20;
		Edip_Draw_Line(Line_1, Line_2);
		point.x = 10;
		point.y += 25;
		Edip_Draw_String(point,"2:");
		point.x = 120;
		Edip_Draw_String(point,"%s",GET_TEXT(S_Abort_payment));
		Line_1.y = point.y + 20;
		Line_2.y = point.y + 20;
		Edip_Draw_Line(Line_1, Line_2);
		break;
	case SCR_PC:
		printf("SCR_PC:[menu header] %s\r\n",GET_TEXT(S_Cant_print_receipt));
		printf("SCR_PC:[menu 1] %s\r\n",GET_TEXT(S_Continue_without_receipt));
		printf("SCR_PC:[menu 2] %s\r\n",GET_TEXT(S_Abort_payment));
		break;
	default:
		break;
	}
}

void SCR_printing_receipt(void){
	Point_t point = {.x=120,.y=10};
	Edip_Clear_Display();
	switch(g_screen_type){
	case SCR_EDIP:
		Edip_Set_Font(&H1Font);
		Edip_Draw_String(point,GET_TEXT(S_Printing_receipt));
		break;
	case SCR_PC:
		printf("SCR_PC: %s\r\n",GET_TEXT(S_Printing_receipt));
		break;
	default:
		break;
	}
	return;
}

PT_THREAD(Quest_print_receipt(Transaction_info_t * trans, process_event_t ev, process_data_t data)){

	PT_BEGIN(&Transaction_sub_pt);
	if(Trans_state_change){
		Trans_state_change(STATE_RECEIPT_QUESTION);
	}
	SCR_receipt_transaction();
	SET_SCREEN_TIMEOUT(20);
	ev = PROCESS_EVENT_NONE;
	while(ev != PROCESS_EVENT_EXIT){
		if(PROCESS_EVENT_SCREEN_TIMEOUT){
			trans->payment_success = false;
			Productlist_clear();
			PT_EXIT(&Transaction_sub_pt);
		}

		if(keypad_pressed(ev)){
			uint8_t key = *((uint8_t *)data);
			if((key == 1)||(key == C_OK)){
				trans->payment_success = true;
				trans->print_receipt = true;
				break;
			}else if(key == 2){
				trans->payment_success = true;
				trans->print_receipt = false;
				break;
			}else if(key == C_RETURN){
				trans->payment_success = false;
				Productlist_clear();
				PT_EXIT(&Transaction_sub_pt);
			}
		}
		PT_YIELD(&Transaction_sub_pt);
	}
	PT_END(&Transaction_sub_pt);
}

PT_THREAD(Quest_no_paper(Transaction_info_t * trans, process_event_t ev, process_data_t data)){

	PT_BEGIN(&Transaction_sub_pt);
	SCR_no_paper();
	SET_SCREEN_TIMEOUT(20);
	ev = PROCESS_EVENT_NONE;
	while(ev != PROCESS_EVENT_EXIT){
		if(PROCESS_EVENT_SCREEN_TIMEOUT){
			trans->payment_success = false;
			Productlist_clear();
			PT_EXIT(&Transaction_sub_pt);
		}

		if(ev == event_Button3x4_pressed){
			uint8_t key = *((uint8_t *)data);
			if((key == 1)||(key == C_OK)){
				trans->payment_success = true;
				trans->print_receipt = false;
				break;
			}else if(key == 2){
				trans->payment_success = false;
				trans->print_receipt = false;
				break;
			}else if(key == C_RETURN){
				trans->payment_success = false;
				Productlist_clear();
				PT_EXIT(&Transaction_sub_pt);
			}
		}
		PT_YIELD(&Transaction_sub_pt);
	}
	PT_END(&Transaction_sub_pt);
}

#endif


