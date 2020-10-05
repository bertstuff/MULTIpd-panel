#ifndef CCV_GLOBAL_H_
#define CCV_GLOBAL_H_

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <Core/protocol/Net/uip.h>
#include <Device/I2C/RFID/RFID_Pass.h>
#include <core/Memory/ringbuf.h>

#define BUFLEN 256
#define OVERALLRESULT_IS_STATE (g_OverallResult == R_UNKNOWN)||(g_OverallResult >= R_STATE_CASHIERDISPLAY)

extern const char CCV_OverallResult_str[20][23];

typedef enum{
	R_UNKNOWN, //0:Procedure not finished yet.
	R_SUCCESS, //1:Complete Success
	R_FAILURE, //2:Complete Failure
	R_PRINTLASTTICKET, //3:Complete Failure. Payment rejected by PINpad because receipt of last transaction has not been printed
	R_DEVICEUNAVAILABLE, //4:Complete Failure. No further request will be successful because a device is temporary unavailable (e.g. busy with PINpad Initiated Function or other request)
	R_ABORTED, //5:Complete failure. The transaction was aborted by cashier or cardholder
	R_TIMEDOUT, //6:Complete Failure. No response from remote host, cardholder timeout or cashier input timeout
	R_FORMATERROR, //7:Complete Failure. The request cannot be handled or mistakenly (unknown) formatted
	R_PARSINGERROR, //8:Complete Failure. The request XML is not well formed
	R_VALIDATIONERROR, //9:Complete Failure. The request XML is not validated against the definition scheme
	R_MISSINGMANDATORYDATA, //10:Complete Failure. The request message is missing mandatory data
	R_CCV_CONNECTION_ERROR, //11:Cant connect to ATM
	R_PRINTER_LOW, //12:Printer has no paper
	R_CONNECTION_ABORTED, //13: ATM starting up

	//State message
	R_STATE_CASHIERDISPLAY, //14
	R_CUSTOMERDISPLAY,		//15
	R_STATE_PRINTER,		//16
	R_STATE_JOURNALPRINTER, //17
	R_STATE_EJOURNAL,		//18
	R_STATE_DELIVERYBOX		//19
}CCV_OverallResult_t;

typedef enum{
	DR_SUCCESS, //Complete Success
	DR_FAILURE, //Complete Failure. Device available, but output failed (e.g. printer paper empty condition)
	DR_DEVICEUNAVAILABLE, //Complete Failure. Device is unavailable (e.g. printer is offline)
	DR_FORMATERROR, //Complete Failure. The request cannot be handled or mistakenly (unknown) formatted
	DR_TIMEDOUT, //Complete Failure. Cashier input timeout
	DR_PARSINGERROR, //Complete Failure. The request XML is not well formed
	DR_VALIDATIONERROR, //Complete Failure. The request XML is not validated against the definition scheme
	DR_MISSINGMANDATORYDATA //Complete Failure. The request message is missing mandatory data
}CCV_DevReqt_Result_t;

typedef enum{
	CardPayment,
	AbortRequest,
	TicketReprint,
	RepeatLastMessage,
	GetCardCircuits,
	PaymentRefund
}CCV_RequestType_t;

extern char Language_str[4][7];

typedef struct{
	int32_t Total_Price;
	Language_t language;
	bool print_receipt;
}payment_t;

typedef struct{
	uip_ipaddr_t CCV_IP_addr;
	PT_THREAD((* Delivery_box)(struct pt * box_pt, bool *Delivery_ok, process_event_t ev, process_data_t data));
	char Workstation_ID[10];
	bool Setup_done;
}CCV_settings_t;

typedef struct{
	bool VISA; //VISA
	bool ECMC; //MasterCard
	bool AMEX; //AmericanExpress
	bool JCB;  //JCB
	bool PRLI; //Prime Line
	bool CMFC; //Comfort card
	bool DC;   //DINERS
	bool MAES; //Maestro
}CCV_CardBrand_t;

extern process_event_t event_new_connection;
extern process_event_t event_CCV;
extern process_event_t event_CCV_state;

extern CCV_settings_t g_CCV_settings;
extern CCV_RequestType_t g_CCV_RequestType;
extern payment_t g_cur_payment;
extern uint8_t g_RequestID_chan1;
extern uint8_t g_RequestID_chan2;
extern CCV_OverallResult_t g_OverallResult;
extern CCV_CardBrand_t CardBrand;
extern uint8_t g_CCV_buffer[BUFLEN];
extern const char CCV_inifile[];
extern bool g_receipt_printed;
extern uint16_t g_ShiftNumber;
#endif /* CCV_GLOBAL_H_ */

