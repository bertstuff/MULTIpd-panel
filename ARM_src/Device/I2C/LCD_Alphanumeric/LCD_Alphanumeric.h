#ifndef IO_LCD_ALPHANUMERIC_H__
#define IO_LCD_ALPHANUMERIC_H__
	#include <Core/Type/type.h>
	#include <stdbool.h>
	#include <stdarg.h>
	#include <Core/Process/process.h>

	#define LCD_MAX_SPEED_HZ 400000

	#ifndef GPIO_ADDR
	#define GPIO_ADDR 0xFE
	#endif

	#define DISPLAY_ON true
	#define DISPLAY_OFF !DISPLAY_ON
	#define CURSOR_ON true
	#define CURSOR_OFF !CURSOR_ON
	#define CURSOR_BLINK true
	#define CURSOR_STATIC !CURSOR_BLINK


	typedef enum{
		SHIFT_DISPALY = 1,
		SHIFT_CURCOR = 0
	}LCD_Shift_Mode;

	typedef enum{
		DIR_LEFT = 0,
		DIR_RIGHT = 1
	}LCD_Direction;

	typedef struct{
		bool RS;
		bool RW;
		uint8_t data;
		uint16_t delay;
	}LCD_data_pack;

	typedef struct{
		bool used;
		Point_t pos;
		uint8_t lenght;
		uint8_t scroll_pos;
		char * text;
	}LCD_scroll_text;

	typedef struct{
		uint8_t Fontsize;
		uint8_t Data_bits;
		uint8_t Data_lines;
	}LCD_function_set_t;

	/*
	 * @brief select current screen
	 * @param addr lcd i2c addres, or use GPIO_ADDR for lcd on gpio
	 *
	 */
	void LCD_Alphanumeric_select(uint8_t addr);

	/*
	 * @brief init lcd
	 * @param addr lcd i2c addres, or use GPIO_ADDR for lcd on gpio
	 * @return Status SUCCESS or ERROR
	 */
	Status LCD_Alphanumeric_Init(uint8_t addr);

	/*
	 * @brief select lcd when using multiple lcd's
	 * @param addr i2c adders  or GPIO_ADDR for gpio
	 */
	void LCD_Alphanumeric_select(uint8_t addr);

	/*
	 * @brief clear display
	 */
	Status LCD_Alphanumeric_Clear_Display(void);

	/*
	 * @brief return curcor to home position
	 */
	Status LCD_Alphanumeric_Cursor_Home(void);

	/*
	 * @brief Select entry mode
	 * @param Mode shift lcd or shift cursor when data added
	 * @param shift left or right
	 */
	Status LCD_Alphanumeric_Entry_Mode(LCD_Shift_Mode Mode,LCD_Direction Dir);

	/*
	 * @brief set display control
	 * @param Display_on ste display on or off
	 * @param Cursor_on set cursor on or off
	 * @param Cursor_blink let cursor blink
	 */
	Status LCD_Alphanumeric_Display_Control(bool Display_on, bool Cursor_on, bool Cursor_blink);

	/*
	 * @brief shift display or cursor
	 * @param mode select display or cursor to shift
	 * @param dir shift direction
	 */
	Status LCD_Alphanumeric_Display_Shift(LCD_Shift_Mode Mode,LCD_Direction Dir);

	/*
	 * @brief set curcor position
	 * @param pos position of cursor
	 */
	Status LCD_Alphanumeric_Set_Cursor_addr(Point_t pos);

	/*
	 *@brief load a special char to the lcd screen
	 *@param mem_loc 0...7 connect to char value to draw this symbol.
	 *@param symbol array with te symbol. (see LCD_16x2_char.h)
	 */
	Status LCD_Alphanumeric_Load_Symbol(uint8_t mem_loc,const uint8_t symbol[8]);

	/**
	  *@brief Draw string to screen
	  *@param format string to draw on screen. See function 'printf' for more information about format possibilities.
	  *@ text between <~text~> blinks and <5#scroll text#> is a 5char scrolling text bar
	  *@param ... Variables included in string.
	  *@return SUCCESS or ERROR
	  **/
	 Status LCD_Alphanumeric_Draw_String(Point_t pos, char * format,...);

	 uint8_t LCD_Alphanumeric_addr(void);
#endif //IO_LCD_ALPHANUMERIC_H__
