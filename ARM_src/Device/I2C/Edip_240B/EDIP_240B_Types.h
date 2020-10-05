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
 *       This file contains all type definitions for the Edip screen. Please include  "EDIP_240B.h" to use the Edip types and functions.
 * \author
 *         Mark van der Heijden <m.vanderheijden@custommate.eu>
 * 
 */
#ifndef I2C_EDIP_240B_TYPES
#define I2C_EDIP_240B_TYPES

  #include <Driver/include/lpc_types.h>
  #include <stdint.h>
  #include <stdbool.h>
  #include <Core/Type/type.h>
 ////////////////////////Typedef enum////////////////
  
  typedef enum{
    A_LEFT = 'L',
    A_CENTER = 'C',
    A_RIGHT = 'R',
    A_UP = 'O',
    A_DOWN = 'U'
  }EDIP_Align_t;
 
  typedef enum{
    R_0DEG = 0,
    R_90DEG = 1,
    R_180DEG = 2,
    R_270DEG = 3
  }EDIP_Rotate_t; 
  
  typedef enum{
    M_SET = 1,
    M_DELETE = 2,
    M_INVERSE = 3,
    M_REPLACE = 4,
    M_INV_REP = 5,
  }EDIP_Mode_t; 
 
  typedef enum{
    B_OFF = 0,
    B_ON = 1,
    B_INVERSE = 2,
  }EDIP_Blink_t; 
  
  typedef enum{
    P_WHITE = 0,
    P_BLACK = 1,
    P_GRAY0 = 2,
    P_GRAY20 = 3,
    P_GRAY40 = 4,
    P_GRAY60 = 5,
    P_GRAY80 = 6,
    P_NOISE = 7,
    P_LINE45 = 8,
    P_LINE135 = 9,
    P_LINE157 = 10,
    P_LINE22 = 11,
    P_RECT45 = 12,
    P_SQUARESMALL = 13,
    P_SQUAREBIG = 14,
    P_SQUAREBIG45 = 15
  }EDIP_Pattern_t; 
  
  typedef enum{
    ALIGN,
    FONT,
    ZOOM_X,
    ZOOM_Y,
    SPACING,
    ANGLE,
    MODE,
    BLINK
  }EDIP_fontset_t;

  typedef enum{
    G_BAR = 0,
    G_BAR_IN_RECT = 1,
    G_LINE = 2,
    G_LINE_IN_RECT = 3,
  }EDIP_BarType_t;   
  
  typedef enum{
    KEY,
    SWITCH
  }EDIP_Button_t;     
  
 ////////////////////////typedef structures////////////////

  typedef struct{
    EDIP_Align_t Align;
    uint8_t Font;
    uint8_t Zoom_X;
    uint8_t Zoom_Y;
    uint8_t Spacing;
    EDIP_Rotate_t Angle;
    EDIP_Mode_t Mode;
    EDIP_Blink_t Blink;
  }EDIP_font_t;
  
  typedef struct{
    EDIP_Align_t place;
    EDIP_BarType_t Type;
    EDIP_Pattern_t Pattern;
    uint8_t Line_width;
  }EDIP_BarGraph_Style_t;
  
  typedef struct{
    struct EDIP_BarGraph_t *next;
    uint8_t BarNr;
    uint8_t Val;
  }EDIP_BarGraph_t;
  
  typedef struct{
    struct EDIP_Touch_t *next;
    uint8_t Nr_press;
    uint8_t Nr_release;
    void (* Press)();
    void (* Release)();
  }EDIP_Touch_t;
  
  typedef struct{
   	uint8_t Bar_nr;
   	uint8_t Value;
   }last_Bargraph_event_t;

#endif
 /**
 * 	@}
 * @}
 **/
