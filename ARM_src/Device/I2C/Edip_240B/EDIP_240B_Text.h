 /**
 * \addtogroup device
 * @{
 */

/**
 * \defgroup EDIP_240B_API
 *
 * EDIP 240B LCD API.
 *
 * @{
 */

/**
 * \file
 *       This file contains functions to display text on the Edip lcd screen. Please include  "EDIP_240B.h" to use these functions
 * \author
 *         Mark van der Heijden <m.vanderheijden@custommate.eu>
 * 
 */
#ifndef I2C_EDIP_240B_Text
#define I2C_EDIP_240B_Text

  #include "EDIP_240B_Types.h"
  
  #include <stdint.h>
  #include <stdbool.h>
  #include <string.h>
  #include <stdarg.h>
  #include <stdio.h>
   
  #ifndef I2C_SPEED_HZ
    #define I2C_SPEED_HZ 400
  #endif

  ///////////////////Font functions////////////////////////////////
  /**
  *@brief Set new font to default
  *@param Font pointer to Font struct 
  **/  
  void Edip_New_Font(EDIP_font_t * Font);
  
  /**
  *@brief Change setting of font
  * -SETTING-      -VALUE-
  * ALIGN           A_LEFT
                    A_CENTER
                    A_RIGHT
    FONT            0..16
    ZOOM_X          1..4
    ZOOM_Y          1..4
    SPACING         pixels between lines
    ANGLE           R_0DEG
                    R_90DEG
    MODE            M_SET
                    M_DELETE
                    M_INVERSE
                    M_REPLACE
                    M_INV_REP
    BLINK           B_OFF
                    B_ON
                    B_INVERSE
  *
  *@param Font pointer to Font struct 
  *@param setting Select setting 
  *@param value of setting
  **/ 
  Status Edip_Change_Font_Setting(EDIP_font_t * Font, EDIP_fontset_t setting, uint8_t value);
  
  /**
  *@brief use font
  *@param Font pointer to Font struct 
  **/ 
  Status Edip_Set_Font(EDIP_font_t * Font);

  ///////////////////String functions////////////////////////////////
  /**
  *@brief Draw string to screen 
  *@param p1 Point on screen to display the string
  *@param format string to draw on screen. See function 'printf' for more information about format possibilities.
  * Some extra format char are implemented. text between ~ char flashes. text between @ char flashes inverse. A | char separate a new line. 
  *@param ... Variables included in string.
  *@return SUCCESS or ERROR
  **/
  Status Edip_Draw_String(Point_t p1, char * format,...);
  
  Status Edip_Draw_String_va_list(Point_t p1, char * format, va_list args);

#endif
 /**
 * 	@}
 * @}
 **/
