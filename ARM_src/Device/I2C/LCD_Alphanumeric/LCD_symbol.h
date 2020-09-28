#ifndef IO_LCD_SYMBOL_H__
#define IO_LCD_SYMBOL_H__

	#include <stdint.h>

	#define LEFT_ARROW  0x7F
	#define RIGHT_ARROW 0x7E

	extern const uint8_t C_Arrow_Down[8];
	extern const uint8_t C_Arrow_up[8];
	extern const uint8_t C_Clock[8];

	extern const uint8_t C_Play_w[8];
	extern const uint8_t C_Play_b[8];
	extern const uint8_t C_Stop_w[8];
	extern const uint8_t C_Stop_b[8];
	extern const uint8_t C_Pauze[8];

	extern const uint8_t C_Euro[8];
	extern const uint8_t C_Hourglass[8];
	extern const uint8_t C_Lock[8];
	const uint8_t C_Progress[6][8];

#endif //IO_LCD_SYMBOL_H__
