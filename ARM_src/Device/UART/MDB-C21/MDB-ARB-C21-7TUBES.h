/*
Peter-Paul Greuter    Navmate 29-5-2009

Deze include is bijna dezelfde als ARB_C21.h , met bijzonderheid dat deze slechts 7 buizen ondersteund ipv 16. Het MDB
protocol ondersteund er namelijk maximaal 16 en de ontvangen berichten staan ook in deze vorm!

    BELANGRIJK!!!!
SerTransOke[2] RxLen[2] en RxBuffer[2] moeten gedeclareerd zijn in de .nc

Zie code ARB-C21.xls voor functie omschrijvingen
Na ca.50 ms kan er gekeken worden of er een bericht is deze zou er dan moeten zijn.
Interne functies geven allemaal een transok boolean als return waarde.

Onder aan de include file staan de when statements waar deze is bij getest
*//////////////////////////////////////////////////////////////////////////////



#ifndef _ARB_C21_h
#define _ARB_C21_h
	#include <stdbool.h>
	#include <stdint.h>
	#include <Core/Peripherals/UART_Dev_Init.h>
	#include <Core/Process/process.h>

	typedef struct{
	  uint8_t TubeEmpty;
	  uint8_t CCdefect;
	  uint8_t Escrow;
	  uint8_t WrongCmdLevel;
	  uint8_t Reserve;
	} MDB_Error_t;

	 typedef struct{
	  uint8_t TubeFull;
	  uint8_t TubeAlmostEmpty;
	  uint8_t CoinRouting;
	  uint8_t CoinReject;
	  uint8_t MesageError;
	  uint8_t PayoutFail;
	  uint8_t DecodeTimeOut;
	  uint8_t Reserve;
	} MDB_Warning_t;

	extern process_event_t event_coin_inserted ;
	extern process_event_t event_coin_payed;
	extern process_event_t event_MDB_state;
	extern MDB_Error_t MDB_Error;
	extern MDB_Warning_t MDB_Warning;
	extern uint8_t CoinValue[7];
	extern uint8_t CoinTube[7];
	extern uint8_t CoinSF;
	extern uint16_t Cash_Box;
	extern int16_t CCgeldIn;
	extern int16_t CCgeldUit;
	extern bool SetupDoneCC;

	PROCESS_NAME(mdb_process);

	void MDB_init(UART_select_t Com);

	void MDB_Ontvang(bool ontvang);
	bool MDB_Betaal(signed long geld);


#endif
