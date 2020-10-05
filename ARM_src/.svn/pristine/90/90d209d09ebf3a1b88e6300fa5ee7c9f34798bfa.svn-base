#include <stdint.h>
#include <string.h>
#include <Processes/Global_UID.h>

UID_t g_Cur_UID;
UID_t g_Empty_UID;

void Set_GLobal_UID(UID_t UID){
	g_Cur_UID = UID;
}

UID_t Get_GLobal_UID(void){
	return g_Cur_UID;
}

void Reset_GLobal_UID(void){
	memset(&g_Cur_UID.UID[0], 0, 7);
	g_Cur_UID.type = NONE_UID;
}
