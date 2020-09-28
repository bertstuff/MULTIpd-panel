#ifndef __SCR_INIT_H__
#define __SCR_INIT_H__
#include <Board/Def_config.h>
#include <Core/Process/process.h>
#if(BOARD_SCREEN == 1)
	#include <Screen/SCR_Text.h>
	#include <Device/I2C/Edip_240B/EDIP_240B.h>
	#include <Device/I2C/LCD_Alphanumeric/LCD_Alphanumeric.h>

	#define C_EURO 128

	extern EDIP_font_t H1Font;
	extern EDIP_font_t StdFont;//15px
	extern EDIP_font_t SmallFont;//10px
	extern EDIP_font_t TinyFont;

	extern struct process *screen_current;
	/*
	 * @brief select a screen
	 * @param Screen_type screen type
	 * @param addr i2c address of screen
	 */
	void SCR_select(Screen_t Screen_type, uint8_t addr);

	/*
	 * @brief initialize screen and set software version on screen
	 */
	void SCR_init(void);

	/*
	 * @brief function to set buzzy variable
	 * @param state set state of busy variable
	 */
	void SCR_set_busy(bool state);

	/*
	 * @brief get status of busy variable
	 */
	bool SCR_is_busy(void);


#endif
	/*
	 * @brief load a screen and exit current screen
	 * @param p next pointer to next screen process
	 */
	void SCR_load(struct process *p);
#endif //__SCR_INIT_H__
