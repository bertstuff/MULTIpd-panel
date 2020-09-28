#include "Devices.h"
#include <Core/Peripherals/UART_Dev_Init.h>
#include <core/Extensions/ini/minIni.h>
#include <Core/protocol/Net/Ejournal/EJournal.h>
#include <Screen/Transaction/SCR_Transaction_Receipt.h>
Devices_available_t g_Devices_available;

/*
Devices_available_t Devices_init(void){

#if (DEVICE_ATM == 1)
	if(g_Devices_available.ATM_available){
		CCV_init();
	}
#endif
#if (DEVICE_CASH == 1)
	if(g_Devices_available.Cash_available){
		UART_select_t uart_cash;
		printf("transaction: Cash available\r\n");
		uart_cash = ini_getuart("Cash_validator","Uart", "Basis_EBM_COM1", "Select uart for Cash validator", inifile_device);
		XBA_init(uart_cash);
	}
#endif
#if (DEVICE_COIN_CHANGER == 1)
	if(g_Devices_available.Coin_changer_available){
		UART_select_t uart_coin;
		printf("transaction: Coin changer available\r\n");
		uart_coin = ini_getuart("MDB","Uart", "Basis_EBM_COM2", "Select uart for DMX", inifile_device);
		MDB_init(uart_coin);
	}
#endif
#if (DEVICE_PAYTER == 1)
	if(g_Devices_available.Payter_available){
		printf("transaction: payter available\r\n");
		RTP_init();
	}
#endif
#if (DEVICE_TG2460_PRINTER == 1)
	if(g_Devices_available.Printer_available){
		UART_select_t uart_printer;
		printf("transaction: printer available\r\n");
		uart_printer = ini_getuart("Receipt_Printer","Uart", "I2C_UART1", "Select uart for printer", inifile_device);
		TG2460H_Init(uart_printer);
		Receipt_init();
	}
#endif
#if (DEVICE_QR == 1)
	if(g_Devices_available.QR_available){
		printf("transaction: QR available\r\n");
		QR_init(false);
	}
#endif
#if (DEVICE_RWD_RFID == 1)
	if(g_Devices_available.Multipass_available == true){
		printf("transaction: RFID available\r\n");
		g_RFID_card = RFID_get_card(RFID_init(0,READER_MIFARE));
		if(g_Devices_available.Multipass_offline == true){
			g_RFID_card = RFID_get_card(RFID_get_reader(0x00));
			g_RFID_card->card_action.Credit_read = true;
			g_RFID_card->card_action.Info_read = true;
			g_RFID_card->card_action.Unlock_code_read = true;
		}
	}
#endif
#if (DEVICE_COIN_ACCEPTOR == 1)
	if(g_Devices_available.Coin_acceptor_available){
		printf("transaction: coin acceptor available\r\n");
		EMP_800_init();
	}
#endif
#if (DEVICE_ELTRA_RFID == 1)
	if(g_Devices_available.EltraRFID_available){
		UART_select_t uart_eltra;
		printf("transaction: Eltra available\r\n");
		uart_eltra = ini_getuart("Eltra","Uart", "Basis_EBM_COM2", "Select uart for printer", inifile_device);
		ELTRA_init(uart_eltra);
	}
#endif
#if ((DEVICE_PAYTER == 1)||(DEVICE_ATM == 1))
	if(g_Devices_available.Payter_available || g_Devices_available.ATM_available){
		process_start(&EJournal_process,NULL);
	}
#endif
	return g_Devices_available;
}*/
