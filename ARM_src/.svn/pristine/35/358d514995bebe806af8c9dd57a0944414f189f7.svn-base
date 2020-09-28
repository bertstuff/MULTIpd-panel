#include <Board/Def_config.h>
#if(PAYING_MODULE == 1)
#include "CCV_global.h"
#include "CCV_Chan1.h"
#include "CCV_Chan2.h"
#include "CCV.h"

#include <Core/Process/Process.h>
#include <Core/protocol/Net/uip_arp.h>
#include <Core/protocol/Net/emac/emac-driver.h>
#include <Core/protocol/Net/Ejournal/EJournal.h>
#include <Core/Timer/etimer.h>
#include <Core/Debug/Debug_Terminal.h>
#include <Global.h>
#include <Core/Extensions/mxml/mxml.h>
#include <Core/Clock/rtc_cal.h>
#include <Core/protocol/Net/psock.h>
#include <Core/Util/array.h>
#include <Device/UART/TG2460H/TG2460H.h>
#include <cr_section_macros.h>
#include <Core/protocol/FAT/ff.h>
#include <Core/Util/math_util.h>

#include <Core/Memory/cmem.h>
#include <core/Extensions/mxml/mxml-memory.h>
#undef DEBUG
#define DEBUG 1

#define BUF_LEN_CHAN1 255

static struct psock ps_CHAN1;
struct etimer Timeout_T1_tm;
struct etimer Timeout_T2_tm;
CCV_CardBrand_t CardBrand;
struct uip_conn * Chan1_connection;

uint8_t g_RequestID_chan1 = 255;
uint8_t g_lost_RequestID_chan1 = 0;
uint8_t g_lost_RequestID_chan2 = 0;
uint16_t g_ShiftNumber;
uint8_t * g_CCV_buffer_chan1;
mxml_node_t * chan1_tree = NULL;


char Language_str[4][7] = {"nl","en","de","fr"};
const char flag_file[] = "SD1:/PAYST.fl";

//Prototypes
PROCESS(CCV_Chan1_process, "Process tcp ip channel 1 ARM->CCV");

//Functions

void CCV_Set_PaymentStartedFlag(bool state){
	FIL file;
	FRESULT rc;
	UINT bw;

	if(state == true){
		rc = f_open( &file,flag_file, FA_CREATE_ALWAYS | FA_WRITE);
		if(rc != FR_OK)
		{
		 printf("CCV(Chan1): Can't create flag file %s (%d) \r\n",flag_file ,rc);
		}else{
			rc = f_write (&file,"PaymentStartedFlag",19,&bw);
			if(rc == FR_OK){
				printf("CCV(Chan1): set PaymentStartedFlag true \r\n");
			}
		}
	}else{
		rc = f_unlink (flag_file);
		if(rc != FR_OK)
		{
		  printf("CCV(Chan1): Can't remove flag file %s (%d) \r\n",flag_file ,rc);
		}else{
			printf("CCV(Chan1): set PaymentStartedFlag false \r\n");
		}
	}

	return;
}

bool CCV_Get_PaymentStartedFlag(void){
	FIL file;
	FRESULT rc;

	bool state;
	//state = RTC_ReadGPREG (LPC_RTC,0) > 0?true:false;

	rc = f_open(&file, flag_file, FA_OPEN_EXISTING | FA_READ);
	if(rc == FR_OK){
		state = true;
	}else{
		state = false;
	}

	printf("CCV(Chan1): get PaymentStartedFlag %s \r\n",(state == true)?"true":"false");
	return state;

}


short unsigned int CCV_ServiceRequest(void * data){
	mxml_node_t *xml; /* <?xml ... ?> */
	mxml_node_t *ServiceRequest; /* <CardServiceRequest> or <ServiceRequest> */
	mxml_node_t *POSdata; /* <POSdata> */
	mxml_node_t *TotalAmount; /* <TotalAmount> */
	mxml_node_t *POSdata_field;

	// <?xml ... ?>
	mxmlSetWrapMargin(0);
	xml = mxmlNewXML("1.0");

	//<CardServiceRequest> or <ServiceRequest>
	if(g_CCV_RequestType == GetCardCircuits){
		ServiceRequest = mxmlNewElement(xml, "ServiceRequest");
	}else{
		ServiceRequest = mxmlNewElement(xml, "CardServiceRequest");
	}
	mxmlElementSetAttr(ServiceRequest,"WorkstationID",g_CCV_settings.Workstation_ID);
	mxmlElementSetAttrf(ServiceRequest,"RequestID","%d",g_RequestID_chan1);

	if(g_CCV_RequestType == CardPayment){
		//Set payment started flag
		g_receipt_printed = false;
		CCV_Set_PaymentStartedFlag(true);
		mxmlElementSetAttr(ServiceRequest,"RequestType","CardPayment");
		printf("CCV(Chan1): Starting CardPayment \r\n");
	}else if(g_CCV_RequestType == TicketReprint){
		mxmlElementSetAttr(ServiceRequest,"RequestType","TicketReprint");
		printf("CCV(Chan1): Starting TicketReprint \r\n");
	}else if(g_CCV_RequestType == AbortRequest){
		mxmlElementSetAttr(ServiceRequest,"RequestType","AbortRequest");
		printf("CCV(Chan1): Starting AbortRequest \r\n");
	}else if(g_CCV_RequestType == RepeatLastMessage){
		mxmlElementSetAttr(ServiceRequest,"RequestType","RepeatLastMessage");
		printf("CCV(Chan1): Starting RepeatlastMessage \r\n");
	}else if(g_CCV_RequestType == GetCardCircuits){
		mxmlElementSetAttr(ServiceRequest,"RequestType","GetCardCircuits");
		printf("CCV(Chan1): Starting GetCardCircuits \r\n");
	}else if(g_CCV_RequestType == PaymentRefund){
		g_receipt_printed = false;
		CCV_Set_PaymentStartedFlag(true);
		mxmlElementSetAttr(ServiceRequest,"RequestType","PaymentRefund");
		printf("CCV(Chan1): Starting PaymentRefund \r\n");
	}
	mxmlElementSetAttr(ServiceRequest,"xmlns","http://www.nrf-arts.org/IXRetail/namespace");

	//<POSdata>
	POSdata = mxmlNewElement(ServiceRequest, "POSdata");
	if((g_CCV_RequestType == CardPayment)||(g_CCV_RequestType == PaymentRefund)){
		mxmlElementSetAttrf(POSdata,"LanguageCode","%s",Language_str[g_cur_payment.language]);
		mxmlElementSetAttrf(POSdata,"ShiftNumber","%d",g_ShiftNumber);
	}
	POSdata_field = mxmlNewElement(POSdata, "POSTimeStamp");
	struct tm rtc_time;
	char time_str[80];
	rtc_cal_gettime(&rtc_time);
	strftime (time_str,80,"%Y-%m-%dT%H:%M:%S",&rtc_time);
	mxmlNewText(POSdata_field, 0,time_str);

	POSdata_field = mxmlNewElement(POSdata, "PrinterStatus");
	TG2460H_Paper_status_t paper_status = TG2460H_Get_paper_status();
	if(paper_status == S_PAPER_OK){
		mxmlNewText(POSdata_field, 0, "Available");
	}else if(paper_status == S_PAPER_LOW){
		mxmlNewText(POSdata_field, 0, "PaperLow");
		printf("CCV(Chan1): paper PaperLow \r\n");
	}else{
		mxmlNewText(POSdata_field, 0, "PaperEmpty");
		printf("CCV(Chan1): paper PaperEmpty \r\n");
	}

	//POSdata_field = mxmlNewElement(POSdata, "JournalPrinterStatus");
	//mxmlNewText(POSdata_field, 0, "Available");

	POSdata_field = mxmlNewElement(POSdata, "E-JournalStatus");
	EJ_Memory_status_t Memory_status = EJ_Memory_status();
	if(Memory_status == S_MEMORY_OK){
		mxmlNewText(POSdata_field, 0, "Available");
	}else{
		mxmlNewText(POSdata_field, 0, "MemoryLow");
	}


	// <TotalAmount>
	if((g_CCV_RequestType == CardPayment)||(g_CCV_RequestType == PaymentRefund)){
		TotalAmount = mxmlNewElement(ServiceRequest, "TotalAmount");
		mxmlElementSetAttr(TotalAmount,"Currency","EUR");
		mxmlNewTextf(TotalAmount, 0, "%.2f", Centen_to_Euro(g_cur_payment.Total_Price));

	}
	mxmlSaveString(xml, &((char *)uip_appdata)[4], UIP_CONF_BUFFER_SIZE,	MXML_NO_CALLBACK);
	mxmlDelete(xml);

	uint8_t i;
	uint16_t pack_len = 0;

	pack_len = strlen(&((char *)uip_appdata)[4]);
	for(i = 0; i < 4 ;i++){ //add the lenght bytes to packet
		((char *)uip_appdata)[i] = pack_len >> ((3 - i)*8);
	}

	return pack_len + 4;
}

void  CCV_RepeatLastMessageResponse(mxml_node_t * tree){
	mxml_node_t * node;
	uint8_t Rec_Request_ID_chan1 = 0;
	uint8_t Rec_Request_ID_chan2 = 0;

	node = mxmlFindElement(tree, tree, "OriginalHeader",NULL, NULL,MXML_DESCEND);
	if (node != NULL){
		char **arr;
		int size;
		bool Reprint = false;

		size = explode(&arr,(char *)mxmlElementGetAttr(node,"RequestID"), '.');
		if(size == 0){
			printf("CCV(Chan1): RepeatLastMessage Received no RequestID\r\n");
			g_OverallResult = R_PARSINGERROR;
		}
		if(size >= 1){
			Rec_Request_ID_chan1 = atoi(arr[0]);
			Reprint = (Rec_Request_ID_chan1 != g_lost_RequestID_chan1) ? true: false;

		}
		if(size >= 2){
			Rec_Request_ID_chan2 = atoi(arr[1]);
			Reprint = (Rec_Request_ID_chan2 != g_lost_RequestID_chan2) ? true: false;
		}
		free(arr);

		node = mxmlFindElement(tree, tree, "OriginalHeader",NULL, NULL,MXML_DESCEND);

		if(Reprint == true){
			printf("CCV(Chan1): RepeatLastMessage Received wrong RequestID invoke reprint cur=%d.%d  recv=%d.%d \r\n",g_lost_RequestID_chan1,g_lost_RequestID_chan2,Rec_Request_ID_chan1,Rec_Request_ID_chan2);
			g_OverallResult = R_PRINTLASTTICKET;
		}else{
			if(g_receipt_printed == false){
				printf("CCV(Chan1): RepeatLastMessage receipt not printed yet\r\n");
				g_OverallResult = R_PRINTLASTTICKET;
			}else{
				printf("CCV(Chan1): RepeatLastMessage no reprint needed\r\n");
			}
		}
	}
	return;
}

void  CCV_GetCardCircuitsResponse(mxml_node_t * tree){
	mxml_node_t * Line = tree;
	char * Brand_name;

	while(1){
		Line = mxmlFindElement(Line, tree, "CardCircuit",NULL, NULL,MXML_DESCEND);
		if(Line == NULL){
			break;
		}
		Brand_name = (char *) mxmlGetOpaque(Line);
		printf("CCV(Chan1): Accept %s (%s)\r\n",(char *)mxmlElementGetAttr(Line,"DisplayName"),Brand_name);
		if(strcmp(Brand_name, "VISA") == 0){
			CardBrand.VISA = true;
		}else if(strcmp(Brand_name, "ECMC") == 0){
			CardBrand.ECMC = true;
		}else if(strcmp(Brand_name, "AMEX") == 0){
			CardBrand.AMEX = true;
		}else if(strcmp(Brand_name, "DC") == 0){
			CardBrand.DC = true;
		}else if(strcmp(Brand_name, "JCB") == 0){
			CardBrand.JCB = true;
		}else if(strcmp(Brand_name, "PRLI") == 0){
			CardBrand.PRLI = true;
		}else if(strcmp(Brand_name, "CMFC") == 0){
			CardBrand.CMFC = true;
		}else if(strcmp(Brand_name, "MAES") == 0){
			CardBrand.MAES = true;
		}
	}
	return;
}
void  CCV_ServiceResponse(mxml_node_t * tree){
	CCV_RequestType_t Cur_Reqtype = TicketReprint;

	if (tree == NULL){
		printf("CCV(Chan1): xml parse error (Error in XML structure)\r\n");
		g_OverallResult = R_PARSINGERROR;
		goto post_OverallResult;
	}
	mxml_node_t * node;
	node = mxmlFindElement(tree, tree, "CardServiceResponse",NULL, NULL,MXML_DESCEND);
	if(node == NULL){
		node = mxmlFindElement(tree, tree, "ServiceResponse",NULL, NULL,MXML_DESCEND);
	}

	if (node != NULL){
		const char * RequestType_str;

		RequestType_str = mxmlElementGetAttr(node,"RequestType");
		if(strstr(RequestType_str,"CardPayment") != NULL){
			Cur_Reqtype = CardPayment;
		}else if(strstr(RequestType_str,"AbortRequest") != NULL){
			Cur_Reqtype = AbortRequest;
		}else if(strstr(RequestType_str,"TicketReprint") != NULL){
			Cur_Reqtype = TicketReprint;
		}else if(strstr(RequestType_str,"RepeatLastMessage") != NULL){
			Cur_Reqtype = RepeatLastMessage;
		}else if(strstr(RequestType_str,"GetCardCircuits") != NULL){
			Cur_Reqtype = GetCardCircuits;
		}else if(strstr(RequestType_str,"PaymentRefund") != NULL){
			Cur_Reqtype = PaymentRefund;
		}
	}

	if (node != NULL){
		uint8_t Rec_Request_ID_chan1 = atoi(mxmlElementGetAttr(node,"RequestID"));

		if(g_RequestID_chan1 != Rec_Request_ID_chan1){
			g_OverallResult = R_PARSINGERROR;
			printf("CCV(Chan1): Error Received wrong RequestID (cur:%d != Recv:%d)\r\n", g_RequestID_chan1, Rec_Request_ID_chan1);
			if(Cur_Reqtype != RepeatLastMessage){
				goto post_OverallResult;
			}
		}else{
			printf("CCV(Chan1):(cur:%d == Recv:%d)\r\n", g_RequestID_chan1, Rec_Request_ID_chan1);
		}
	}else{
		printf("CCV(Chan1): xml parse error (Can't find RequestID)\r\n");
		g_OverallResult = R_PARSINGERROR;
		goto post_OverallResult;
	}

	if (node != NULL){
		const char * Rec_OverallResult = mxmlElementGetAttr(node,"OverallResult");
		if(strcmp(Rec_OverallResult, "Success") == 0){
			g_OverallResult = R_SUCCESS;
		}else if(strcmp(Rec_OverallResult, "Failure") == 0){
			g_OverallResult = R_FAILURE;
		}else if(strcmp(Rec_OverallResult, "PrintLastTicket") == 0){
			g_OverallResult = R_PRINTLASTTICKET;
		}else if(strcmp(Rec_OverallResult, "DeviceUnavailable") == 0){
			g_OverallResult = R_DEVICEUNAVAILABLE;
		}else if(strcmp(Rec_OverallResult, "Aborted") == 0){
			g_OverallResult = R_ABORTED;
		}else if(strcmp(Rec_OverallResult, "TimedOut") == 0){
			g_OverallResult = R_TIMEDOUT;
		}else if(strcmp(Rec_OverallResult, "FormatError") == 0){
			g_OverallResult = R_FORMATERROR;
		}else if(strcmp(Rec_OverallResult, "ParsingError") == 0){
			g_OverallResult = R_PARSINGERROR;
		}else if(strcmp(Rec_OverallResult, "ValidationError") == 0){
			g_OverallResult = R_VALIDATIONERROR;
		}else if(strcmp(Rec_OverallResult, "MissingMandatoryData") == 0){
			g_OverallResult = R_MISSINGMANDATORYDATA;
		}else{
			g_OverallResult = R_PARSINGERROR;
		}
		printf("CCV(Chan1): OverallResult: %s \r\n",Rec_OverallResult);
	}else{
		printf("CCV(Chan1): xml parse error (Can't find OverallResult)\r\n");
		g_OverallResult = R_PARSINGERROR;
	}

	//Message specific handlers
	if((g_CCV_RequestType == CardPayment)||(g_CCV_RequestType == PaymentRefund)){
		//clear payment started flag
		CCV_Set_PaymentStartedFlag(false);
	}else if(Cur_Reqtype == RepeatLastMessage){
		CCV_RepeatLastMessageResponse(tree);
	}else if(Cur_Reqtype == GetCardCircuits){
		CCV_GetCardCircuitsResponse(tree);
	}else if(Cur_Reqtype == AbortRequest){
		return;
	}

post_OverallResult:
	process_post(PROCESS_BROADCAST, event_CCV, &g_OverallResult);
	return;
}

void sax1_cb(mxml_node_t *node, mxml_sax_event_t event, void *data){
	 if (event == MXML_SAX_ELEMENT_OPEN){
		mxmlRetain(node);
	 }else if (event == MXML_SAX_DIRECTIVE){
		mxmlRetain(node);
	}else if (event == MXML_SAX_DATA){
	  {
		if (mxmlGetRefCount(mxmlGetParent(node)) > 1)
		{
		  //If the parent was retained, then retain this data node as well.
		  mxmlRetain(node);
		}
	  }
	}
}


static PT_THREAD(handle_chan1_connection(struct psock *p)){
	PSOCK_BEGIN(p);

	g_lost_RequestID_chan1 = g_RequestID_chan1;
	g_lost_RequestID_chan2 = g_RequestID_chan2;

	g_RequestID_chan1 ++;
	g_RequestID_chan2 = 0;

	static uint16_t pack_len = 0;
	uint8_t i;

	//Send new CardServiceRequest or ServiceRequest packet to ATM

	PSOCK_GENERATOR_SEND(p,CCV_ServiceRequest,NULL);
	//Receive answer from ATM

	PSOCK_WAIT_UNTIL(p,psock_newdata(p) || (g_CCV_RequestType == AbortRequest));

	if(g_CCV_RequestType == AbortRequest){
		PSOCK_GENERATOR_SEND(p,CCV_ServiceRequest,NULL);
		PSOCK_WAIT_UNTIL(p,psock_newdata(p));
		pack_len = 0;
		for(i = 0; i < 4 ;i++){ //get packet lenght
		   pack_len += *((uint8_t *)(uip_appdata + i)) << ((3 - i)*8);
		}
		PSOCK_READ_XML(p,&chan1_tree,sax1_cb,pack_len);
		CCV_ServiceResponse(chan1_tree);
		mxmlDelete(chan1_tree);
		chan1_tree = NULL;
		PSOCK_WAIT_UNTIL(p,psock_newdata(p));
	}

	pack_len = 0;
	for(i = 0; i < 4 ;i++){ //get packet lenght
	   pack_len += *((uint8_t *)(uip_appdata + i)) << ((3 - i)*8);
	}
	PSOCK_READ_XML(p,&chan1_tree,sax1_cb,pack_len);

#if DEBUG > 0
	printf("CCV(Chan1): New packet received from CCV lenght %d+4\r\n",pack_len);
#endif
	CCV_ServiceResponse(chan1_tree);
	mxmlDelete(chan1_tree);
	chan1_tree = NULL;
	PSOCK_CLOSE(p);
	PSOCK_END(p);
}

PROCESS_THREAD(CCV_Chan1_process, ev, data)
{
	  //Timeout timers
	  if(ev == PROCESS_EVENT_TIMER){
		  if(etimer_expired(&Timeout_T1_tm)&&(OVERALLRESULT_IS_STATE)){
			  printf("CCV(Chan1): Timeout T1\r\n");
			  g_OverallResult = R_DEVICEUNAVAILABLE;
			  process_post(PROCESS_BROADCAST, event_CCV, &g_OverallResult);
			  process_start(&CCV_Startup_process, NULL);
		  }
		  if(etimer_expired(&Timeout_T2_tm)&&(OVERALLRESULT_IS_STATE)){
			  printf("CCV(Chan1): Timeout T2\r\n");
			  g_OverallResult = R_TIMEDOUT;
			  process_post(PROCESS_BROADCAST, event_CCV, &g_OverallResult);
		      process_start(&CCV_Startup_process, NULL);
		  }
	  }

	  PROCESS_BEGIN();
	  while (ev != PROCESS_EVENT_EXIT){ //loop till exit
		  PROCESS_WAIT_EVENT();
///////////Create connection//////////
		  if(ev == event_new_connection){
#if DEBUG > 0
			  printf("CCV(Chan1): Start connecting to CCV\r\n");
#endif
			  g_OverallResult = R_UNKNOWN;
			  tcp_connect(&g_CCV_settings.CCV_IP_addr, UIP_HTONS(4100),NULL);
		  }

//////////Receive new data////////////
		  if(ev == tcpip_event){
			  if(uip_aborted()&&(g_OverallResult != R_SUCCESS)){
				printf("CCV(Chan1): Connection to CCV aborted\r\n");
				g_OverallResult = R_CONNECTION_ABORTED;
				process_post(PROCESS_BROADCAST, event_CCV, &g_OverallResult);
			  }else if(uip_timedout()){
				printf("CCV(Chan1): Connection to CCV timeout\r\n");
				g_OverallResult = R_CCV_CONNECTION_ERROR;
				process_post(PROCESS_BROADCAST, event_CCV, &g_OverallResult);
			  } else if(uip_connected()) {
#if DEBUG > 0
				printf("CCV(Chan1): ARM connected to CCV\r\n");
#endif
				g_CCV_buffer_chan1 = cmem_malloc(BUF_LEN_CHAN1);
				PSOCK_INIT(&ps_CHAN1, g_CCV_buffer_chan1, BUF_LEN_CHAN1);
				etimer_set(&Timeout_T1_tm, CLOCK_SECOND * 60 * 5);//timeout T1 5min
				etimer_set(&Timeout_T2_tm, CLOCK_SECOND * 50);//timeout T2 50sec
				//Loop when connection is establish
				do {
				  PROCESS_WAIT_EVENT_UNTIL((ev == tcpip_event)||(ev == PROCESS_EVENT_TIMER));
				  //New tcp/ip traffic
				  if(ev == tcpip_event){
					  if(etimer_expired(&Timeout_T1_tm)||etimer_expired(&Timeout_T2_tm)){
						  uip_close();
					  }else{
						  handle_chan1_connection(&ps_CHAN1);
					  }
				  }
				} while(!(uip_closed() || uip_aborted() || uip_timedout()));
#if DEBUG > 0
				printf("CCV(Chan1): ARM to CCV connection closed.\r\n");
#endif
				if(chan1_tree != NULL){
				   mxmlDelete(chan1_tree);
				   chan1_tree = NULL;
				}
				cmem_free(g_CCV_buffer_chan1);
				//when Connection closed to early unknown result
				if(OVERALLRESULT_IS_STATE){
					printf("CCV(Chan1): Connection closed to early\r\n");
					g_OverallResult = R_CCV_CONNECTION_ERROR;
					process_post(PROCESS_BROADCAST, event_CCV, &g_OverallResult);
					process_start(&CCV_Startup_process, NULL);
				}
				process_start(&EJournal_process, NULL);
			 }
		  }
	  }
	  PROCESS_END();
}
#endif
