#ifndef __GLOBAL_UID_H__
#define __GLOBAL_UID_H__

#include <stdint.h>

typedef enum{
	RFID_UID,
	QR_UID,
	PIN_UID,
	NONE_UID = 255
}UID_type_t;

typedef struct{
	UID_type_t type;
	uint8_t UID[7];
}UID_t;

extern UID_t g_Cur_UID;
extern UID_t g_Empty_UID;

/*
 * @brief set global card or QR UID
 */
void Set_GLobal_UID(UID_t UID);

/*@brief get global UId*/
UID_t Get_GLobal_UID(void);

/*
 * @brief set global uid to 0
 */
void Reset_GLobal_UID(void);

#endif //__GLOBAL_UID_H__

