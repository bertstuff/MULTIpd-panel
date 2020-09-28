#ifndef _RFID_PASS_
#define _RFID_PASS_

#include <Core/Thread/pt.h>
#include <Core/Type/type.h>
#include <Device/I2C/RFID/RFID.h>

extern struct pt RFID_pass_pt;

//--SECTOR 1-- (Not secured)
//-BLOCK 0 UID-
//-BLOCK 1 Simple card check
#define UNLOCK_CODE_BLOCK 1
//-BLOCK 2 User info
#define INFO_BLOCK 2
#define LANGUAGE_BYTE 0
#define MOD_NR_BYTE 1
#define START_END_DATE 2
#define AUTH_LEV_BYTE 10
#define CARD_VERSION_BYTE 11
//-BLOCK 3 Security keys sector0 (default)
//--SECTOR 1-- (secured)
//-BLOCK 4
#define CREDIT_BLOCK 4
#define CREDIT_BYTE 0
#define USER_NR_BYTE 5
#define TRANS_NR_BYTE 6
//-BLOCK 5 undef
//-BLOCK 6 undef
//-BLOCK 7 Security keys sector1 (secured)

#define CURRENT_CARD_V 2

PT_THREAD(_RFID_Set_Unlock_Code(RFID_reader_t * reader, process_event_t ev, process_data_t data));
PT_THREAD(_RFID_Get_Unlock_Code(RFID_reader_t * reader, process_event_t ev, process_data_t data));
PT_THREAD(_RFID_Set_Info(RFID_reader_t * reader, process_event_t ev, process_data_t data));
PT_THREAD(_RFID_Get_Info(RFID_reader_t * reader, process_event_t ev, process_data_t data));
PT_THREAD(_RFID_Set_Credit(RFID_reader_t * reader, process_event_t ev, process_data_t data));
PT_THREAD(_RFID_Get_Credit(RFID_reader_t * reader, process_event_t ev, process_data_t data));

/*
 * Set unlock code on pass
 */
#define PROCESS_PT_RFID_Set_Unlock_code(READER) 	PROCESS_PT_SPAWN(&RFID_pass_pt, _RFID_Set_Unlock_Code(READER,ev,data))

/*
 * Get unlock code from pass
 */
#define PROCESS_PT_RFID_Get_Unlock_code(READER) 	PROCESS_PT_SPAWN(&RFID_pass_pt, _RFID_Get_Unlock_Code(READER,ev,data))

/*
 * Set User info on pass
 */
#define PROCESS_PT_RFID_Set_Info(READER) 			PROCESS_PT_SPAWN(&RFID_pass_pt, _RFID_Set_Info(READER,ev,data))

/*
 * Get User info from pass
 */
#define PROCESS_PT_RFID_Get_Info(READER) 			PROCESS_PT_SPAWN(&RFID_pass_pt, _RFID_Get_Info(READER,ev,data))

/*
 * Set Credits on pass
 */
#define PROCESS_PT_RFID_Set_Credit(READER) 			PROCESS_PT_SPAWN(&RFID_pass_pt, _RFID_Set_Credit(READER,ev,data))

/*
 * Get Credits from pass
 */
#define PROCESS_PT_RFID_Get_Credit(READER) 			PROCESS_PT_SPAWN(&RFID_pass_pt, _RFID_Get_Credit(READER,ev,data))



#endif //_RFID_PASS_
