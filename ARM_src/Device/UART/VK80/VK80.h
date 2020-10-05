/**
 * \addtogroup device
 * @{
 */

/**
 * \defgroup VK80_API
 *
 * The keypad API has functions to communicate with the ticket printer
 *
 * @{
 */

/**
 * \file
 * This file contains functions for the VK80
 * \author
 *         Mark van der Heijden <m.vanderheijden@custommate.eu>
 *
 */
#ifndef VK80_H_
#define VK80_H_
	#include <stdarg.h>
	#include <stdint.h>
	#include <stdbool.h>
	#include <string.h>
	#include <stdio.h>
	#include <Driver/include/lpc17xx_uart.h>
	#include <Core/Peripherals/UART_Dev_Init.h>

	#define _ALIGN_L_		"\ea0"
	#define _ALIGN_C_		"\ea1"
	#define _ALIGN_R_		"\ea2"
	#define _ROT_90DEG_OFF_	"\eV0"
	#define _ROT_90DEG_ON_	"\eV1"
	#define _BOLD_OFF_		"\eE0"
	#define _BOLD_ON_		"\eE1"
	#define _D_STRIKE_OFF_	"\eG0"
	#define _D_STRIKE_ON_	"\eG1"
	#define _ULINE_OFF_		"\e-0"
	#define _ULINE_1PX_		"\e-1"
	#define _ULINE_2PX_		"\e-2"
    #define _SCRIPT_OFF_	"\e40"
	#define _SCRIPT_ON_		"\e41"

	#define CPI_FONTA_11	0
    #define CPI_FONTA_15	1
	#define CPI_FONTA_20	2
	#define CPI_FONTB_15	0
	#define CPI_FONTB_20	1

	#define FONTA 'A'
	#define FONTB 'B'

	typedef enum{
	  C_NORMAL = 0,
	  C_DOUBLE = 1,
	  C_TRIPLE = 2,
	  C_QUADRUPLE = 3,
	  C_5x = 4,
	  C_6x = 5,
	  C_7x = 6,
	  C_8x = 7
	}VK80_Size_t;

	typedef enum{
	  T_0PX = 0,
	  T_1PX = 1,
	  T_2PX = 2
	}VK80_Thick_t;

	typedef struct{
		bool F_bold;
		VK80_Thick_t F_underline;
		bool F_double_strike;
		VK80_Size_t Width;
		VK80_Size_t Height;
		uint8_t cpi;
		char Font;
	}VK80_Font_t;

	typedef enum{
		S_PAPER_OK,
		S_PAPER_LOW,
		S_PAPER_EMPTY,
		S_DEVICE_UNAVAILABLE
	}VK80_Paper_status_t;


	typedef enum{
		H_Not_printed = 0,
		H_Above_BC = 1,
		H_Below_BC = 2,
		H_AboveBelow_BC = 3
	}VK80_HRI_t;

	typedef enum{
		F_UPC_A = 0,
		F_UPC_E = 1,
		F_EAN13 = 2,
		F_EAN8 = 3,
		F_CODE39 = 4,
		F_ITF = 5,
		F_CODABAR = 6,
		F_CODE93 = 7,
		F_CODE128 = 8,
		F_CODE32 = 20
	}VK80_BC_Format_t;

	typedef struct{
		VK80_HRI_t Position_HRI; //Human Readable Interpretation
		bool Font_A; 				//Font A or B for HRI
		uint8_t Height; 			//(96 = 12mm)
		VK80_BC_Format_t Format;
	}VK80_Barcode_t;

	typedef struct{
		uint8_t dot_size; //2..24 pixels per dot
	}VK_QRcode;

	extern VK80_Paper_status_t VK80_status;

	/*
	 * @brief init printer
	 * @uart Select uart output
	 * @return Status ERROR SUCCESS
	 */
	Status VK80_Init(UART_select_t Com);

	/*
	 * @brief Set font for printer
	 * @param Font Font settings
	 * @return
	 */
	void VK80_Set_font(VK80_Font_t* Font);


	/*
	 * @brief set font for TG1260 printer
	 * @param Font 'A' or 'B'
	 */
	Status TG1260H_Set_Font(char Font);

	/*
	 * @brief set font size
	 * @param W Width of font
	 * @param H Height of font
	 */
	Status VK80_Set_Font_Size(VK80_Size_t W, VK80_Size_t H);

	/*
	 * @brief set char per inch
	 * @param cpi 0..2
	 */
	Status VK80_Set_Font_cpi(uint8_t cpi);

	/*
	 * @brief Print string on paper
	 * @Format format string see function printf
	 * @return Status ERROR SUCCESS
	 */
	Status VK80_Print_string(char * format,...);

	/*
	 * @brief get paper status
	 * @return paper status
	 */
	VK80_Paper_status_t VK80_Get_paper_status(void);

	/*
	 * @brief cut paper
	 * @return Status ERROR SUCCESS
	 */
	Status VK80_Cut_paper(void);

	/*
	 * @brief Print barcode on paper
	 * @param Barcode Barcode settings
	 * @param Data_str data string
	 * @return Status ERROR SUCCESS
	 */
	Status VK80_Print_Barcode(VK80_Barcode_t Barcode, char * Data_str);

	/*
	 * @brief print QRcode
	 * @param Qrcode Qrcode settings
	 * @param data_str data
	 */
	Status VK80_Print_QRcode(VK_QRcode Qrcode, char * Data_str);

	/*
	 * @brief print DATAMATRIX
	 * @param DATAMATRIX DATAMATRIX settings
	 * @param DATAMATRIX data
	 */
	Status VK80_DATAMATRIX_QRcode(VK_QRcode Qrcode, char * Data_str);

	/*
	 * @brief set left margin
	 * @param inch offset in inches
	 * @return Status ERROR SUCCESS
	 */
	Status VK80_left_margin(double inch);

	/*
	 * @brief set printing position
	 * @param inch offset in inches
	 * @return Status ERROR SUCCESS
	 */
	Status VK80_absolute_position(double inch);

	/*
	 * @brief Store monochrome BMP image in memory
	 * @param refnr storage nr.
	 * @param filename name of bmp file
	 * @return Status ERROR SUCCESS
	 */
	Status VK80_Download_BMP_Logo(uint8_t refnr,  char *filename);

	/*
	 * @brief print iamge from memory
	 * @param refnr storage nr
	 * @return Status ERROR SUCCESS
	 */
	Status VK80_Print_Logo(uint8_t refnr);

#endif
 /**
 * 	@}
 * @}
 **/

