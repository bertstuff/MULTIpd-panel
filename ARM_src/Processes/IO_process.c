#include "IO_process.h"
#include "config.h"
#include <Core/Timer/etimer.h>
#include <Device/I2C/8574/I2C_8574.h>
#include <Device/IO/IO/EBM_Multipass_io.h>
#include <Driver/include/lpc17xx_gpio.h>

#ifndef IO_POLL_INTERVAL
	#define IO_POLL_INTERVAL CLOCK_SECOND
#endif

/*types*/

struct IO_poll_list_t{
	struct IO_poll_list_t * next;
	Device_info_t * Device;
	void (*poll_function)(bool state_change, struct IO_input_list_t * input);
	bool prev_state;
	struct IO_input_list_t * input;
}IO_poll_list_t;

/*global variable*/
PROCESS(IO_poll_process, "IO poll process");
struct IO_poll_list_t * IO_poll_list = NULL;
struct IO_output_list_t * IO_output_list = NULL;
struct IO_input_list_t * IO_input_list = NULL;

/*prototypes*/
void IO_poll_device_init(Device_info_t * Device);
void IO_input_init(Device_info_t * Device,uint32_t pin);
void IO_output_init();
void IO_poll_device(Device_info_t * Device);
Status IO_change_output(struct IO_output_list_t * output,bool state);


/*funtions*/
Device_info_t * IO_add_device(IO_device_t Device, uint8_t Device_addr){
	Device_info_t * Device_handle;

	Device_handle = malloc(sizeof(Device_info_t));
	if(Device_handle == NULL){
		return NULL;
	}
	Device_handle->Device = Device;
	Device_handle->Device_addr = Device_addr;
	Device_handle->polled = false;
	IO_poll_device_init(Device_handle);
	return Device_handle;
}

void IO_input_init(Device_info_t * Device,uint32_t pin){
	switch(Device->Device){
	case D_GPIO_PORT:
		GPIO_SetDir(Device->Device_addr,(1<<pin), DIR_INPUT);
		break;
	default:
		break;
	}
}

Status IO_input_add(Device_info_t * Device, uint32_t pin, uint32_t ref_nr){
	struct IO_input_list_t * input;

	input = malloc(sizeof(struct IO_input_list_t));
	if(input == NULL){
		return ERROR;
	}

	input->Device = Device;
	input->pin = pin;
	input->ref_nr = ref_nr;
	input->next = NULL;
	input->init = true;

	if(IO_input_list == NULL){
		IO_input_list = input;
	}else{
		input->next = IO_input_list;
		IO_input_list = input;
	}
	IO_input_init(Device,pin);
	return SUCCESS;
}

Status IO_input_poll_add(Device_info_t * Device, uint32_t pin,void (*poll_function)(bool state_change, struct IO_input_list_t * input),uint32_t ref_nr){
	struct IO_poll_list_t * info;
	struct IO_input_list_t * list;

	info = malloc(sizeof(struct IO_poll_list_t));
	if(info == NULL){
		return ERROR;
	}

	if(IO_input_add(Device,pin,ref_nr) == ERROR){
		return ERROR;
	}

	//find just created input.
	list = IO_input_list;
	while(list != NULL){
		if(list->ref_nr == ref_nr){
			break;
		}
		list = list->next;
	}

	info->Device = Device;
	info->poll_function = poll_function;
	info->input = list;
	info->prev_state = false;
	info->next = NULL;

	if(IO_poll_list == NULL){
		IO_poll_list = info;
	}else{
		info->next = IO_poll_list;
		IO_poll_list = info;
	}
	IO_input_init(Device,pin);
	process_start(&IO_poll_process,NULL);
	return SUCCESS;
}

void IO_output_init(Device_info_t * Device,uint32_t pin){
	switch(Device->Device){
	case D_GPIO_PORT:
		GPIO_SetDir(Device->Device_addr,(1<<pin), DIR_OUTPUT);
		break;
	default:
		break;
	}
}

Status IO_output_add(Device_info_t * Device, uint32_t pin, uint32_t ref_nr){
	struct IO_output_list_t * info;

	info = malloc(sizeof(struct IO_output_list_t));
	if(info == NULL){
		return ERROR;
	}

	info->Device = Device;
	info->ref_nr = ref_nr;
	info->pin = pin;
	info->time = NULL;
	info->next = NULL;

	if(IO_output_list == NULL){
		IO_output_list = info;
	}else{
		info->next = IO_output_list;
		IO_output_list = info;
	}
	IO_output_init(Device, pin);
	return SUCCESS;
}

bool IO_get_input_state(uint32_t ref_nr){
	struct IO_input_list_t * list;
	list = IO_input_list;
	while(list != NULL){
		if(list->ref_nr == ref_nr){
			IO_poll_device(list->Device);
			list->state = (list->Device->input_state & (1<<list->pin));
			return list->state;
		}
		list = list->next;
	}
	return false;
}


Status IO_set_output_state(uint32_t ref_nr, bool state){
	Status error = ERROR;
	struct IO_output_list_t * list;
	list = IO_output_list;
	while(list != NULL){
		if(list->ref_nr == ref_nr){
			error |= IO_change_output(list,state);
		}
		list = list->next;
	}
	return error;
}

Bool IO_get_output_state(uint32_t ref_nr){
	struct IO_output_list_t * list;
	list = IO_output_list;
	while(list != NULL){
		if(list->ref_nr == ref_nr){
			return list->state;
		}
		list = list->next;
	}
	return false;
}

Status IO_set_output_state_delayed(uint32_t ref_nr, bool state, clock_time_t time){
	Status error = ERROR;
	struct IO_output_list_t * list;
	list = IO_output_list;
	while(list != NULL){
		if(list->ref_nr == ref_nr){
			if(list->time == NULL){
				IO_time_t * time = malloc(sizeof(IO_time_t));
				list->time = time;
			}
			list->time->delay_state = state;
			timer_set(&list->time->timer, time);
			process_add_poll_event(&IO_poll_process);
			error |= SUCCESS;
		}
		list = list->next;
	}
	return error;
}

PROCESS_THREAD(IO_poll_process, ev, data)
{
	struct IO_poll_list_t * list;
	struct IO_output_list_t * list_out;

	static struct etimer Poll_tm;

    if(etimer_expired(&Poll_tm)) {
      etimer_set(&Poll_tm, IO_POLL_INTERVAL);
    }

	PROCESS_BEGIN();
	while(1){
		PROCESS_WAIT_EVENT();
		//reset all poll flags
		list = IO_poll_list;
		while(list != NULL){
			list->Device->polled = false;
			list = list->next;
		}
		//poll input devices
		list = IO_poll_list;
		while(list != NULL){
			if(list->Device->polled == false){
				IO_poll_device(list->Device);
			}
			list->prev_state = list->input->state;
			list->input->state = (list->Device->input_state & (1<<list->input->pin));
			list->poll_function(list->prev_state != list->input->state, list->input);
			list->input->init = false;
			list = list->next;
		}

		//poll output;
		list_out = IO_output_list;
		while(list_out != NULL){
			if(list_out->time != NULL){
				if(timer_expired(&list_out->time->timer)){
					IO_change_output(list_out,list_out->time->delay_state);
					free(list_out->time);
					list_out->time = NULL;
				}else{
					if(timer_remaining(&list_out->time->timer) < CLOCK_SECOND){
						etimer_set(&Poll_tm, timer_remaining(&list_out->time->timer));
					}
				}
			}

			list_out = list_out->next;
		}
	}
	PROCESS_END();
}
void IO_poll_device_init(Device_info_t * Device){
	switch(Device->Device){
	case D_GPIO_PORT:

		break;
	case D_4BIT_IO:
		if(Device->Device_addr < 10){
			Device->Device_addr = (0x70) + (Device->Device_addr << 1);
		}
		Device->output_state = 0xff;
		I2C_8574_Init();
		break;
	case D_EBM_DOUCHE:
		EBM_Multipass_io_init();
	default:
		Device->error = ERROR;
		break;
	}
	return;
}


void IO_poll_device(Device_info_t * Device){
	switch(Device->Device){
	case D_GPIO_PORT:
		Device->error = SUCCESS;
		Device->input_state = GPIO_ReadValue(Device->Device_addr);
		break;
	case D_4BIT_IO:
		Device->error = I2C_8574_ReadInput (Device->Device_addr, (uint8_t *)&Device->input_state, 0xF0);
		Device->input_state = (((~Device->input_state)>>4) & 0x0F);
		break;
	case D_EBM_DOUCHE:
		Device->error = SUCCESS;
		Device->input_state = EBM_Multipass_Read_All_inputs();
	default:
		Device->error = ERROR;
		break;
	}
	Device->polled = true;
}

Status IO_change_output(struct IO_output_list_t * output,bool state){
	switch(output->Device->Device){
	case D_GPIO_PORT:
		if(state == true){
			GPIO_SetValue(output->Device->Device_addr, (1<<output->pin));
		}else{
			GPIO_ClearValue(output->Device->Device_addr, 1<<output->pin);
		}
		output->Device->output_state = GPIO_ReadValue(output->Device->Device_addr);
		output->state = (output->Device->output_state & (1<<output->pin));
		break;
	case D_4BIT_IO:
		if(state == true){
			output->Device->output_state &= ~(1 << output->pin);
			output->Device->error = I2C_8574_WriteOutput (output->Device->Device_addr, (uint8_t *) &output->Device->output_state, 0xF0);
		}else{
			output->Device->output_state |= (1 << output->pin);
			output->Device->error = I2C_8574_WriteOutput (output->Device->Device_addr, (uint8_t *) &output->Device->output_state, 0xF0);
		}
		break;
	case D_EBM_DOUCHE:
		if(state == true){
			EBM_Multipass_Set_output(output->pin);
		}else{
			EBM_Multipass_Clear_output(output->pin);
		}
	default:
		output->Device->error = ERROR;
		break;
	}
	return output->Device->error;
}

