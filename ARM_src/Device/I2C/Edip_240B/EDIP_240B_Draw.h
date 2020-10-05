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
 *      This file contains functions to draw lines on the Edip lcd screen. Please include  "EDIP_240B.h" to use these functions
 * \author
 *         Mark van der Heijden <m.vanderheijden@custommate.eu>
 * 
 */
#ifndef I2C_EDIP_240B_DRAW
#define I2C_EDIP_240B_DRAW
  
  #include "EDIP_240B_Types.h"
  #include <stdint.h>
  #include <stdbool.h>
  #include <string.h>

  ///////////////////Draw functions////////////////////////////////
  /**
  *@brief Define line thickness 
  *@param Width_px Width of line in pixels
  *@param Height_px Height of line in pixels
  *@return SUCCESS or ERROR
  **/
  Status Edip_Set_Line_Thickness(uint8_t Width_px, uint8_t Height_px);
  
  /**
  *@brief Set draw mode  
  *@param Mode How draw function is preformed. possible values: M_SET, M_DELETE and M_INVERSE
  *@return SUCCESS or ERROR
  **/
  Status Edip_Set_Draw_mode(EDIP_Mode_t Mode);
  
  /**
  *@brief Draw Rectangle with lines
  *@param p1 First point
  *@param p2 Second point
  *@return SUCCESS or ERROR
  **/  
  Status Edip_Draw_Rectangle(Point_t p1, Point_t p2);
  
  /**
  *@brief Draw a filed Rectangle
  *@param p1 First point
  *@param p2 Second point
  *@return SUCCESS or ERROR
  **/
  Status Edip_Draw_Fill_Rectangle(Point_t p1, Point_t p2);

  /**
  *@brief Draw Line
  *@param p1 First point
  *@param p2 Second point
  *@return SUCCESS or ERROR
  **/  
  Status Edip_Draw_Line(Point_t p1, Point_t p2);
  
  /**
  *@brief Draw a line with multiple points  
  *@param p_array Array with points
  *@param points The value of points in p_array
  *@return SUCCESS or ERROR
  *@code //Example
  * EDIP_Point_t Line[4] = {{.x=2,.y=2},{.x=12,.y=12},{.x=2,.y=52},{.x=52,.y=52}};
  * Edip_Draw_Multipoint_line(Line, 4);
  **/  
  Status Edip_Draw_Multipoint_line(Point_t * p_array, uint8_t points);
  
  /**
  *@brief Draw point
  *@param p1 point to place point
  *@return SUCCESS or ERROR
  **/  
  Status Edip_Draw_Point(Point_t p1);
  
#endif
 /**
 * 	@}
 * @}
 **/
