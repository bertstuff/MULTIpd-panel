#include <Board/Def_config.h>
#if (PAYING_MODULE == 1)
#include "SCR_Transaction_init.h"
#include "SCR_Transaction_Receipt.h"
#include "SCR_Transaction_statistics.h"
#include <Core/Extensions/ini/minIni.h>
#include <Device/Net/CCV/CCV.h>
#include <Device/UART/MDB-C21/MDB-ARB-C21-7TUBES.h>
#include <Device/UART/XBA/XBA.h>
#include <Device/UART/TG2460H/TG2460H.h>
#include <Device/UART/ELTRA/ELTRA.h>
#include <Device/I2C/LV3095/LV3095.h>
#include <Device/I2C/RFID/RFID.h>
#include <Device/Net/payter/RTP.h>
#include <Device/I2C/EMP_800/EMP_800.h>
#include <Core/protocol/Net/Ejournal/EJournal.h>
#include <Device/Devices.h>

extern int32_t g_max_change_value;
extern bool g_bill_accept[4];
extern int32_t g_min_change_level[4];

void Transaction_read_settings(Transaction_info_t * trans);

void Transaction_init(Transaction_info_t * trans){
	Transaction_statistics_load();
	Devices_available(&trans->Device_available);
	Transaction_read_settings(trans);
	return;
}

void Transaction_read_settings(Transaction_info_t * trans){
#if (DEVICE_CASH == 1)
	if(trans->Device_available.Cash_available){
		g_min_change_level[0] = ini_getl("Bill accept","5euro_Change_level",500,"Change level for accepting 5euro bill",inifile_machine);
		g_min_change_level[1] = ini_getl("Bill accept","10euro_Change_level",1000,"Change level for accepting 10 euro bill",inifile_machine);
		g_min_change_level[2] = ini_getl("Bill accept","20euro_Change_level",2000,"Change level for accepting 20 euro bill",inifile_machine);
		g_min_change_level[3] = ini_getl("Bill accept","50euro_Change_level",5000,"Change level for accepting 50 euro bill",inifile_machine);
		g_max_change_value = ini_getl("Bill accept","Max_return_value",2000,"Max change value",inifile_machine);


		g_bill_accept[0] = ini_getbool("Bill accept","5euro_Accept",true,"Accept 5 euro bill" ,inifile_machine);
		g_bill_accept[1] = ini_getbool("Bill accept","10euro_Accept",true,"Accept 10 euro bill" ,inifile_machine);
		g_bill_accept[2] = ini_getbool("Bill accept","20euro_Accept",true,"Accept 20 euro bill" ,inifile_machine);
		g_bill_accept[3] = ini_getbool("Bill accept","50euro_Accept",false,"Accept 50 euro bill" ,inifile_machine);
	}
#endif
}
#endif
