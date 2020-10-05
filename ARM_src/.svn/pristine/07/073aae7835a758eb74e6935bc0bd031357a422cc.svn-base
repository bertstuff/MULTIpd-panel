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
 *        This file contains functions to draw images on the Edip lcd screen. Please include  "EDIP_240B.h" to use these functions
 * \author
 *         Mark van der Heijden <m.vanderheijden@custommate.eu>
 * 
 */
#ifndef I2C_EDIP_240B_Image
#define I2C_EDIP_240B_Image

  #include "EDIP_240B_Types.h"
  
  #include <stdint.h>
  #include <stdbool.h>

// the pixels of a edip screen are not square. use w 116% h 100%

//////////////Draw image//////////////////////////////////////
Status Edip_Load_Image(Point_t p1, uint8_t Image_nr );

Status Edip_Load_extern_Image(Point_t p1, char *filename);

//////////////Image settings//////////////////////////////////////
Status Edip_Image_Zoom(uint8_t Zoom_X, uint8_t Zoom_Y);
Status Edip_Image_Angle(EDIP_Rotate_t Angle);
Status Edip_Image_Mode(EDIP_Mode_t Mode);
Status Edip_Image_Blink(EDIP_Blink_t Blink);

#endif
 /**
 * 	@}
 * @}
 **/
