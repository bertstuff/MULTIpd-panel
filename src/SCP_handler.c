#include <Core/protocol/Net/SCP/SCP_Comm.h>
#include <Core/protocol/Net/SCP/SCP_msg.h>
#include <Core/Extensions/ini/minIni.h>
#include <Global.h>
#include <stdint.h>
#include "Energy_point.h"

SCP_Data_t msg_var_int_send_handler(SCP_Device_t * Device);

void SCP_handlers_init(void){
	SCP_add_new_handler( msg_var_int_send, msg_var_int_send_handler);
}

SCP_Data_t msg_var_int_send_handler(SCP_Device_t * Device){
	char * name = SCP_msg_var_int__varname(Device->Cur_Packet);
	int16_t array_nr;
	if(strcmp ( name, "Energy_point") == 0){
		struct Energy_point_t Energy_point;
		Energy_point.device = Device->Info.SCP_devID;
		Energy_point.number = SCP_msg_var_int_send__value(Device->Cur_Packet);
		Energy_point_new(Energy_point);
		return SCP_msg_Ok();
	}
	array_nr = SCP_array_number(name);
	if(array_nr >= 0){
		if(Energy_point_exist(array_nr)){
			Energy_point_data_t * Energy_data;
			Energy_data = Energy_point_data(array_nr);
			if(strncmp ( name, "State",5) == 0){
				if(Energy_data->State != SCP_msg_var_int_send__value(Device->Cur_Packet)){
					Energy_data->State = SCP_msg_var_int_send__value(Device->Cur_Packet);
					printf("State changed to %d\r\n", Energy_data->State);
					process_post(PROCESS_BROADCAST,event_state_update,NULL);
				}
				return SCP_msg_Ok();
			}else if(strncmp ( name, "MaxCurrent",10) == 0){
				Energy_data->MaxAmpere = SCP_msg_var_int_send__value(Device->Cur_Packet);
				return SCP_msg_Ok();
			}else if(strncmp ( name, "Watt",4) == 0){
				Energy_data->Power = SCP_msg_var_int_send__value(Device->Cur_Packet);
				return SCP_msg_Ok();
			}else if(strncmp ( name, "MaxWattHour",11) == 0){
				Energy_data->MaxWH = SCP_msg_var_int_send__value(Device->Cur_Packet);
				return SCP_msg_Ok();
			}else if(strncmp ( name, "CurrentWattHour",14) == 0){
				Energy_data->WH = SCP_msg_var_int_send__value(Device->Cur_Packet);
				return SCP_msg_Ok();
			}else if(strncmp ( name, "Current",7) == 0){
				Energy_data->Ampere = SCP_msg_var_int_send__value(Device->Cur_Packet);
				return SCP_msg_Ok();
			}
		}
	}

	return SCP_msg_Error();
}
