#ifndef __IO_PROCESS_H__
#define __IO_PROCESS_H__

#include <Core/Process/process.h>
#include <Driver/include/lpc_types.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Core/Timer/timer.h>
#include <Core/Type/type.h>
#include "IO_process.h"


#define IN1 0
#define IN2 1
#define IN3 2
#define IN4 3

#define OUT1 0
#define OUT2 1
#define OUT3 2
#define OUT4 3

typedef enum{
	D_GPIO_PORT,
	D_4BIT_IO,
	D_EBM_DOUCHE
}IO_device_t;

typedef struct{
	IO_device_t Device;
	uint8_t Device_addr;
	Status error;
	bool polled;
	uint32_t input_state;
	uint32_t output_state;
}Device_info_t;

typedef struct{
	struct timer timer;
	bool delay_state;
}IO_time_t;

struct IO_input_list_t{
	struct IO_input_list_t * next;
	Device_info_t * Device;
	uint32_t ref_nr;
	bool state;
	uint32_t pin;
	bool init;
}IO_input_list_t;

struct IO_output_list_t{
	struct IO_output_list_t * next;
	Device_info_t * Device;
	uint32_t ref_nr;
	bool state;
	uint32_t pin;
	IO_time_t * time;
}IO_output_list_t;

/*
 * @brief add new device
 * @param Device Device type
 * @param  Device_addr i2c addr
 */
Device_info_t * IO_add_device(IO_device_t Device, uint8_t Device_addr);

/*
 * @brief add input
 * @param Device handle to device
 * @param pin pin to check
 * @param ref_nr referentie number
 */
Status IO_input_add(Device_info_t * Device, uint32_t pin, uint32_t ref_nr);

/*
 * @brief add output
 * @param Device handle to device
 * @param pin pin to check
 * @param ref_nr referentie number
 */
Status IO_output_add(Device_info_t * Device, uint32_t pin, uint32_t ref_nr);

/*
 * @brief Add new polled input
 * @param Device handle to device
 * @param pin pin to check
 * @param state_change_function function to call when state change
 * @param ref_nr referentie number
 */
Status IO_input_poll_add(Device_info_t * Device, uint32_t pin,void (*poll_function)(bool state_change, struct IO_input_list_t * input),uint32_t ref_nr);

/*
 * @brief Add a polled function
 * @param poll_function function to call
 */
Status IO_function_poll_add(void (*poll_function)(bool state, uint32_t ref_nr));

/*
 * @brief get input state
 * @param ref_nr referentie number
 */
bool IO_get_input_state(uint32_t ref_nr);

/*
 * @brief set output state
 * @param ref_nr referentie number
 * @param state state of output
 */
Status IO_set_output_state(uint32_t ref_nr, bool state);

/*
 * @brief get output state
 * @param ref_nr referentie number
 */
Bool IO_get_output_state(uint32_t ref_nr);
/*
 * @brief set output state delayed
 * @param ref_nr referentie number
 * @param state state of output
 * @param time delay time
 */
Status IO_set_output_state_delayed(uint32_t ref_nr, bool state, clock_time_t time);


#endif//__IO_PROCESS_H__
