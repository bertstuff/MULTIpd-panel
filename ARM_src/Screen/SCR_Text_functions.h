#ifndef __SCR_TEXT_FUNCTIONS_H__
#define __SCR_TEXT_FUNCTIONS_H__
	#include <stdarg.h>
	#include <Core/Type/type.h>

	typedef enum{
		SCR_EDIP = 0,
		SCR_LCD_2X16 = 1,
		SCR_PC = 2,
	}Screen_t;
	extern Screen_t g_screen_type;
	//#include "Global.h"
	#include <Core/Extensions/ini/minIni.h>
	extern Language_t g_Language;
	extern Language_t g_Language_def;

	/*
	 * @brief return text from SD card.
	 */
	#define H_TEXT(NAME)					char * NAME(Language_t language, Screen_t Screen_type, ...)
	#define	C_TEXT(NAME)					char * NAME(Language_t language, Screen_t Screen_type, ...){return ini_text(#NAME, language, Screen_type);}
	#define	C_STATIC_TEXT(NAME,TEXT)		char * NAME(Language_t language, Screen_t Screen_type, ...){return TEXT;}
	#define F_TEXT(NAME)			 		char * (* NAME)(Language_t language, Screen_t Screen_type, ...)
	#define GET_TEXT(NAME, ...)			 	NAME(g_Language,g_screen_type,##__VA_ARGS__)
	#define GET_LANG_TEXT(NAME, LANG, ...)	NAME(LANG,g_screen_type,##__VA_ARGS__)
	#define REF_TEXT(NAME)			 		NAME


	bool lanuage_exsist(Screen_t Screen_type, Language_t language);
	Language_t lanuage_next(Screen_t Screen_type, Language_t current);
#endif //__SCR_TEXT_FUNCTIONS_H__
