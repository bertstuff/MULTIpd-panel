#ifndef _DEV_RESET_H__
#define _DEV_RESET_H__

	#include <stdbool.h>
	#include <stdint.h>

	/*
	 * @brief reset device immediately
	 */
	void Hard_reset(void);

	/*
	 * @brief reset device on a appropriate moment
	 */
	void Save_reset(void);

	/*
	 * @brief is reset enabled?
	 */
	bool Save_reset_enabled(void);
	/*
	 * @brief poll this function on a appropriate moment to check is the was a save reset request.
	 */
	void Save_reset_poll(void);

#endif //_DEV_RESET_H__
