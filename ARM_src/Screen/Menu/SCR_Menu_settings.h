#include <Board/Def_config.h>
#if(MENU_MODULE == 1)

#ifndef __SCR_MENU_SETTINGS_H__
#define __SCR_MENU_SETTINGS_H__

	#include "SCR_Menu_func.h"
	#include "SCR_Menu_handler.h"
	#include <stdbool.h>
	#include <stdint.h>

	bool SCR_load_setting(struct Menu_t * Menu);

	bool SCR_save_setting(struct Menu_t * Menu);

#endif
#endif
