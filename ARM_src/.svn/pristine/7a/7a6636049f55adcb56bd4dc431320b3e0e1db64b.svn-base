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
#ifndef I2C_EDIP_240B_TOUCH
#define I2C_EDIP_240B_TOUCH

	#include "EDIP_240B_Types.h"

	#include <stdint.h>
	#include <stdbool.h>
	#include <Core/Process/process.h>

	/*
	 * @brief touch event. last button in data
	 */
	extern process_event_t event_edip_touch;

	/*
	 * @brief touch event. data points to a last_Bargraph_event_t struct with the bar number and value
	 */
	extern process_event_t event_edip_Bargraph_touch;
	//Global functions

	/*
	 * @brief remove touch handler
	 * @param touch_nr press or release number
	 */
	Status Edip_Delete_Touch(uint8_t touch_nr);

	/*
	 * @brief remove all touch handlers
	 */
	Status Edip_Delete_Touch_all(void);

	/*
	 * @brief Add touch button to screen. create a event_edip_touch event
	 * @param p1 Begin point
	 * @param p2 End point
	 * @param press_nr event nr when pressing button
	 * @param press_nr event nr when releasing button
	 * @param format string to present on button. For more information see printf function
	 */
	Status Edip_Touch_Button(EDIP_Button_t Type, Point_t p1, Point_t p2, uint8_t press_nr, uint8_t release_nr, char * format,...);

	/*
	 * @brief Add touch Image to screen. create a event_edip_touch event
	 * @param p1 Begin point
	 * @param image_nr Internal image number
	 * @param press_nr event nr when pressing button
	 * @param press_nr event nr when releasing button
	 * @param format string to present on button. For more information see printf function
	 */
	 Status Edip_Touch_Image(EDIP_Button_t Type, Point_t p1, uint8_t image_nr, uint8_t press_nr, uint8_t release_nr, char * format,...);

	 /*
	  * @brief change status of touch button
	  * @param touch_nr press or release nr of button
	  * @status Enable or disable button status
	  */
	 Status Edip_Touch_Set_Status(uint8_t touch_nr, bool status);

	 /*
	  * @brief make bargraph touchable. create
	  * @param Bar bargraph stucture
	  */
	 Status Edip_Touch_graph(EDIP_BarGraph_t * Bar);
	 /*
	  * @brief start Radio button group
	  * @param groupNr group number 1-255
	  */
	 Status Edip_Radiogroup_Start(uint8_t groupNr);
	 /*
	  * @brief end the radio group
	  */

	 Status Edip_Radiogroup_End(void);
	/*
	 * @brief set font of button
	 * @param Font Font settings
	 */
	Status Edip_Set_Label_Font(EDIP_font_t * Font);

	//Local functions
	void Edip_Touch_Init(void);
	Status Exec_Touch_handler(uint8_t Key );
	Status Exec_Bargraph_Touch_handler(uint8_t Bar_nr , uint8_t Value);
#endif
 /**
 * 	@}
 * @}
 **/
