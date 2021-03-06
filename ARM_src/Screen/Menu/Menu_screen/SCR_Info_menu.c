#include <Board/Def_config.h>
#if(MENU_MODULE == 1)

#include <config.h>
#include <Core/Extensions/ini/minIni.h>
#include <Core/Memory/IAP.h>
#include <Core/Type/type.h>
#include <stdint.h>
#include <stdio.h>
#include <Screen/Menu/SCR_Menu_func.h>
#include <Screen/SCR_Text.h>
#include <Core/Util/build_defs.h>
#include <version.h>
#include <core/Clock/rtc_cal.h>
#include <Core/protocol/Net/ntp/ntpd.h>
#include <Core/Clock/Time_functions.h>
//#include <Screen/SCR_Text_functions.h>
#include <version.h>

void SCR_Info_Menu(struct Menu_t * Menu){
	struct Menu_t *	Menu_info;

	struct Menu_item_t * Item_info;


	Item_info = menu_add_menu_item(Menu,REF_TEXT(S_info));

	Menu_info = menu_new_text_menu(REF_TEXT(S_info), Item_info);
	char * S_Serial_Number(Language_t language, Screen_t Screen_type, ...){
		char * buffer = get_textbuffer(80);
		uint32_t Serial_Number[5];
		read_serial_number(Serial_Number);
		sprintf(buffer,"%s:\r\n%08X\r\n%08X\r\n%08X\r\n%08X",GET_TEXT(S_info_Serial_nr),Serial_Number[1],Serial_Number[2],Serial_Number[3],Serial_Number[4]);
		return buffer;
	}

	menu_add_text_item(Menu_info,REF_TEXT(S_Serial_Number));

	char * info_Software(Language_t language, Screen_t Screen_type, ...){
		char * buffer = get_textbuffer(80);
		sprintf(buffer,"%s:\r\n%s.%s%s\r\n\n%d-%d",GET_TEXT(S_info_Software),VERSION_STRING,Build_Revision,Version_state,get_build_date_number(),get_build_time_number());
		return buffer;
	}

	menu_add_text_item(Menu_info,REF_TEXT(info_Software));

	char * info_Product_nr(Language_t language, Screen_t Screen_type, ...){
		char * buffer = get_textbuffer(80);
		sprintf(buffer,"%s:\r\n%s",GET_TEXT(S_info_Product_nr),PRODUCTNR);
		return buffer;
	}

	menu_add_text_item(Menu_info,REF_TEXT(info_Product_nr));

	char * info_Product_name(Language_t language, Screen_t Screen_type, ...){
		char * buffer = get_textbuffer(80);
		sprintf(buffer,"%s:\r\n%s",GET_TEXT(S_info_Product_name),PRODUCT_NAME);
		return buffer;
	}

	menu_add_text_item(Menu_info,REF_TEXT(info_Product_name));


	char * info_Time(Language_t language, Screen_t Screen_type, ...){
		struct tm pFullTime;
		struct tm * Time;
		rtc_cal_gettime(&pFullTime);
		Time = LocalTime(&pFullTime);
		char * buffer = get_textbuffer(80);
		sprintf(buffer,"Time:%d:%d\r\nDate:%d-%d-%d\r\n%s%s",Time->tm_hour,Time->tm_min,Time->tm_mday,Time->tm_mon+1,Time->tm_year+1900,Time_set?"NTP ":"",RTC_ok?"RTC ":"");
		return buffer;
	}

	menu_add_text_item(Menu_info,REF_TEXT(info_Time));

}

#endif
