#include <Board/Def_config.h>
#if(PAYING_MODULE == 1)
#include "CCV_global.h"
#include "CCV.h"
#include "CCV_Chan1.h"
#include "CCV_Chan2.h"

#include <Core/Process/Process.h>
#include <Core/protocol/Net/uip_arp.h>
#include <Core/protocol/Net/dhcp/dhcpc.h>
#include <Core/protocol/Net/emac/emac-driver.h>
#include <Core/Timer/etimer.h>
#include <Core/Debug/Debug_Terminal.h>
#include <Global.h>
#include <Core/Extensions/mxml/mxml.h>
#include <Core/Peripherals/Net_Dev_Init.h>
#include <Core/Extensions/ini/minIni.h>
#include <Core/Util/U_string.h>
#include <Device/UART/TG2460H/TG2460H.h>
#include <Core/Memory/list.h>
#include <Core/Debug/Error_check.h>
#include <Screen/Transaction/SCR_Transaction_products.h>
#include <Screen/SCR_Text_functions.h>
#include <cr_section_macros.h>

//#include <cr_section_macros.h>

/////////global variables////////
process_event_t event_new_connection;
process_event_t event_CCV;
process_event_t event_CCV_state;
struct etimer com_delay_tm;
struct ringbuf g_ring_buffer;
__BSS(RAM2) uint8_t g_CCV_buffer[BUFLEN];
payment_t g_cur_payment;
CCV_settings_t g_CCV_settings;
CCV_OverallResult_t g_OverallResult;
CCV_RequestType_t g_CCV_RequestType;

const char CCV_OverallResult_str[20][23] = {
"UNKNOWN               ",
"SUCCESS               ",
"FAILURE               ",
"PRINT LAST TICKET     ",
"DEVICE UNAVAILABLE    ",
"ABORTED               ",
"TIMEDOUT              ",
"FORMAT ERROR          ",
"PARSING ERROR         ",
"VALIDATION ERROR      ",
"MISSING MANDATORY DATA",
"CONNECTION ERROR      ",
"PAPER ERROR		   ",
"CONNECTION ABORTED    ",
"CASHIERDISPLAY        ",
"CUSTOMERDISPLAY       ",
"PRINTER               ",
"JOURNALPRINTER        ",
"EJOURNAL              ",
"DELIVERYBOX           ",
};

/////////prototypes//////
PROCESS(CCV_Startup_process, "CCV startup process");

/////////functions////////

PROCESS_THREAD(CCV_Startup_process, ev, data)
{
	 PROCESS_BEGIN();
	 if(CCV_Get_PaymentStartedFlag() == true){
		 printf("CCV: payment already started Repeatlast message \r\n");
		 while(1){
			 //Handle Repeatlastmessage
			 //Give the ATM some time
			 etimer_set(&com_delay_tm,CLOCK_SECOND * 4);
			 PROCESS_WAIT_UNTIL(etimer_expired(&com_delay_tm));

			 printf("RepeatLastMessage\r\n");
			 CCV_RepeatLastMessage();
			 do{
				 PROCESS_WAIT_EVENT_UNTIL(ev == event_CCV);
				 printf("CCV:event %s \r\n",CCV_OverallResult_str[g_OverallResult]);
			 }while(OVERALLRESULT_IS_STATE);
			 if(g_OverallResult == R_SUCCESS){
				 break;
			 }
			 if(g_OverallResult != R_PRINTLASTTICKET){
				 continue;
			 }

			 //Handle Reprint receipt
			 do{
				 //Give the ATM some time
				 etimer_set(&com_delay_tm,CLOCK_SECOND * 3);
				 PROCESS_WAIT_UNTIL(etimer_expired(&com_delay_tm));

				 printf("CCV: reprint\r\n");
			 	 CCV_Reprint_Receipt();
			 	 do{
					 PROCESS_WAIT_EVENT_UNTIL(ev == event_CCV);
					 printf("CCV:event %s \r\n",CCV_OverallResult_str[g_OverallResult]);
				 }while(OVERALLRESULT_IS_STATE);
			 }while(g_OverallResult == R_DEVICEUNAVAILABLE);

			 if((g_OverallResult == R_SUCCESS)||(g_OverallResult == R_FAILURE)){
				break;
			 }
		 }
		 //end of recovering
		 CCV_Set_PaymentStartedFlag(false);
	 }else{
		 printf("CCV: normal startup executed \r\n");
	 }


	 //Get accepted cards
	 while(1){
		 //Give the ATM some time
		 etimer_set(&com_delay_tm,CLOCK_SECOND);
		 PROCESS_WAIT_UNTIL(etimer_expired(&com_delay_tm));

		 CCV_GetCardCircuits();
		 do{
			 PROCESS_WAIT_EVENT_UNTIL(ev == event_CCV);
		 }while(OVERALLRESULT_IS_STATE);
		 if(g_OverallResult == R_SUCCESS){
			break;
		 }
	 }
	 PROCESS_END();
}

bool CCV_ini_load(void){
  //Communication
  ini_getipaddr("ATM", "CCV_IP", "10.0.0.24","Ip of ATM:", &g_CCV_settings.CCV_IP_addr, inifile_device);//192.168.0.101
  printf("CCV: OPP-C60 ip: %d.%d.%d.%d\r\n",uip_ipaddr_to_quad(&g_CCV_settings.CCV_IP_addr));
  ini_gets("ATM","Workstation_ID","POS1","ATM Id:",g_CCV_settings.Workstation_ID,10,inifile_device);
  g_ShiftNumber = ini_getl("ATM","ShiftNumber",1,"ShiftNumber:",inifile_device);
  printf("CCV: Workstation ID:%s ShiftNumber:%d\r\n",g_CCV_settings.Workstation_ID,g_ShiftNumber);
  return true;
}

void CCV_init(void){
	printf("CCV: CCV_init\r\n");
	g_Devices_available.ATM_available = ini_getbool("Device","ATM_available",true,"ATM available:",inifile_device);
	if(g_Devices_available.ATM_available == false){
		return;
	}
	g_CCV_settings.Setup_done = CCV_ini_load();

	event_new_connection = process_alloc_event(); //Claim a event number
	event_CCV = process_alloc_event(); //Claim a event number
	event_CCV_state = process_alloc_event(); //Claim a event number

	error_check_add(TYPE_ENUM,&g_OverallResult,ERROR_CCV);
	error_check_ignore_enum_val(ERROR_CCV,R_SUCCESS);
	error_check_ignore_enum_val(ERROR_CCV,R_STATE_CASHIERDISPLAY);
	error_check_ignore_enum_val(ERROR_CCV,R_CUSTOMERDISPLAY);
	error_check_ignore_enum_val(ERROR_CCV,R_STATE_PRINTER);
	error_check_ignore_enum_val(ERROR_CCV,R_STATE_JOURNALPRINTER);
	error_check_ignore_enum_val(ERROR_CCV,R_STATE_EJOURNAL);
	error_check_ignore_enum_val(ERROR_CCV,R_STATE_DELIVERYBOX);

	//Start device
	process_start(&CCV_Chan1_process, NULL);
	process_start(&CCV_Chan2_process, NULL);
	process_start(&CCV_Startup_process, NULL);
	return;
}

bool CCV_startup_done(void){
	return process_is_running(&CCV_Startup_process) == 0?true:false;
}


void CCV_Start_Connection(void){
	//check setup
	if(g_CCV_settings.Setup_done == false){
		printf("CCV setup not done\r\n");
		g_OverallResult = R_FAILURE;
		process_post(PROCESS_BROADCAST, event_CCV, &g_OverallResult);
		return;
	}

	//start connection
	process_post(&CCV_Chan1_process ,event_new_connection ,&g_cur_payment);
	process_poll(&CCV_Chan1_process);
}

void CCV_CardPayment(PT_THREAD((* Delivery_box)(struct pt * box_pt, bool *Delivery_ok, process_event_t ev, process_data_t data)),bool print_receipt){
	if(CCV_enabled == false){
		return;
	}
	g_CCV_settings.Delivery_box = Delivery_box; //Define deliverybox function
	//Check total price
	if(Productlist_total_price(&g_cur_payment.Total_Price) == false){
		printf("CCV: No products\r\n");
		g_OverallResult = R_FAILURE;
		process_post(PROCESS_BROADCAST, event_CCV, &g_OverallResult);
		return;
	}

	//Start payment
	g_CCV_RequestType = CardPayment;
	g_cur_payment.language = g_Language;
	g_cur_payment.print_receipt = print_receipt;
	CCV_Start_Connection();
	return;
}

void CCV_PaymentRefund(PT_THREAD((* Delivery_box)(struct pt * box_pt, bool *Delivery_ok, process_event_t ev, process_data_t data))){
	if(CCV_enabled == false){
		return;
	}
	g_CCV_settings.Delivery_box = Delivery_box; //Define deliverybox function
	//Check total price
	if(Productlist_total_price(&g_cur_payment.Total_Price) == false){
		printf("CCV: No products\r\n");
		g_OverallResult = R_FAILURE;
		process_post(PROCESS_BROADCAST, event_CCV, &g_OverallResult);
		return;
	}

	//Start payment
	g_CCV_RequestType = PaymentRefund;
	g_cur_payment.language = g_Language;
	g_cur_payment.print_receipt = true;
	CCV_Start_Connection();
	return;
}


void CCV_Reprint_Receipt(void){
	if(CCV_enabled == false){
		return;
	}
	//Start Reprint receipt
	g_CCV_RequestType = TicketReprint;
	g_cur_payment.print_receipt = true;
	CCV_Start_Connection();
	return;
}

void CCV_RepeatLastMessage(void){
	if(CCV_enabled == false){
		return;
	}
	//Start RepeatLastMessage
	g_CCV_RequestType = RepeatLastMessage;
	CCV_Start_Connection();
	return;
}

void CCV_GetCardCircuits(void){
	if(CCV_enabled == false){
		return;
	}
	//Start GetCardCircuits
	g_CCV_RequestType = GetCardCircuits;
	CCV_Start_Connection();
	return;
}

void CCV_AbortRequest(void){
	if(CCV_enabled == false){
		return;
	}
	//Start GetCardCircuits
	g_CCV_RequestType = AbortRequest;
	CCV_Start_Connection();
	return;
}

CCV_OverallResult_t CCV_Get_OverallResult(void){
	return g_OverallResult;
}

CCV_CardBrand_t CCV_Get_CardBrand(void){
	return CardBrand;
}

void CCV_increase_ShiftNumber(void){
	g_ShiftNumber++;
	if(g_ShiftNumber > 999){
		g_ShiftNumber = 1;
	}
	ini_putl("ATM","ShiftNumber", g_ShiftNumber, NULL, inifile_device);
}

uint16_t CCV_get_ShiftNumber(void){
	return g_ShiftNumber;
}
#endif
