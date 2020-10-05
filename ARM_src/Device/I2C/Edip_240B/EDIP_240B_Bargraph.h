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
 *        This file contains functions to draw a bar graph to the Edip lcd display. Please include  "EDIP_240B.h" to use these functions
 * \author
 *         Mark van der Heijden <m.vanderheijden@custommate.eu>
 * 
 */
#ifndef I2C_EDIP_240B_BARGRAPH
#define I2C_EDIP_240B_BARGRAPH

  #include "EDIP_240B_Types.h"
  
  #include <stdint.h>
  #include <stdbool.h>

   ///////////////////Global functions////////////////////////////////
   /**
  *@brief Set settings of Style structure.
  *@param Style Pointer to Style structure to set.
  *@param place The Aligning of the graph. Only(A_LEFT, A_RIGHT, A_UP, A_DOWN)
  *@param Type Type of graph.
  *@param Pattern pattern of bar. use with type(G_BAR, G_BAR_IN_RECT)
  *@param Line_width use with type(G_LINE, G_LINE_IN_RECT)
  *@return SUCCESS or ERROR
  **/
  Status Edip_New_Bar_Graph_Style(EDIP_BarGraph_Style_t * Style, EDIP_Align_t place, EDIP_BarType_t Type, EDIP_Pattern_t Pattern, uint8_t Line_width);
  
   /**
  *@brief Draw new Graph
  *@param Bar Pointer to bar handle
  *@param Style Style settings
  *@param p1 first point to draw graph between 
  *@param p2 second point to draw graph between 
  *@param Low_Val value of 0% of graph 
  *@param High_Val value of 100% of graph 
  *@return SUCCESS or ERROR
  **/  
  Status Edip_New_Bar_Graph(EDIP_BarGraph_t * Bar, EDIP_BarGraph_Style_t Style, Point_t p1, Point_t p2, uint8_t Low_Val, uint8_t High_Val);
  
  /**
  *@brief Update Graph when value is changed 
  *@param Bar Pointer to bar handle
  *@param Value New value of graph 
  */  
  Status Edip_Bar_Graph_Value(EDIP_BarGraph_t * Bar, uint8_t Value);
  
  /**
  *@brief Redraw graph completely 
  *@param Bar Pointer to bar handle
  */
  Status Edip_Bar_Graph_Redraw(EDIP_BarGraph_t * Bar);
  
  /**
  *@brief Delete Graph
  *@param Bar Pointer to bar handle
  *@param ClearScreen true: clear graph from screen false :keeps graph on screen. Only handle is deleted  
  */ 
  Status Edip_Bar_Graph_Delete(EDIP_BarGraph_t * Bar, bool ClearScreen);
 

#endif
 /**
 * 	@}
 * @}
 **/
