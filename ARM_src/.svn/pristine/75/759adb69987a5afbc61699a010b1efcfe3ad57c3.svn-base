#include "MDB-ARB-C21-7TUBES.h"
#include <stdbool.h>
#include <Device/Devices.h>
#include <core/Extensions/ini/minIni.h>
#include <Core/Peripherals/UART_Dev_Init.h>
#include <core/Timer/etimer.h>
#include <Driver/include/lpc17xx_uart.h>
#include <stdio.h>
#include <string.h>

#define MDB_time_out  CLOCK_SECOND/5
#define MDB_BUFFER_LENGHT 60
//timers
struct etimer tmMDB_timeout;
struct etimer tmMDB_poll;
struct etimer tmMDB_event_poll;
struct etimer tmMDB_Decode;

//extern gebruiken:
int16_t    CCgeldIn,CCgeldUit=0;    // 0 maken als info verwerkt is.
uint8_t    CoinValue[7]={0,0,0,0,0,0,0};     		// Waarde van munt eenheid in CoinSF //MDB PROTOCOL ONDERSTEUND ER 16! LET OP 2euro munt is er ook maar wordt niet in een tubes ontvangen
uint8_t    CoinTube[7]={0,0,0,0,0,0,0};    		// Aantal van munt eenheid in de Tube van de machine
uint8_t    CoinTubeMAX[7]={75,60,55,50,50,0,0}; 	//Later deze in de EEPROM geldt alleen voor de cashflow 690

uint16_t   Cash_Box=0;      //in 5centen
uint8_t	CoinSF,             //Coin Scaling Factor, geeft aan wat een eenheid is.
		TubesPresent;        //Geeft aan welke munten geaccepteerd worden in tubes

process_event_t event_coin_inserted ;
process_event_t event_coin_payed;
process_event_t event_MDB_state;

//Intern gebruik:
typedef enum tMDBcmdARB{
  MDB_None=0,
  MDB_PollStatus,
  MDB_ResetCC,
  MDB_ReadSetting,
  MDB_GetTubeLevel,
  MDB_PayCoins,
  MDB_AcceptCoins,
  MDB_PayOut,
  MDB_PayedCoins,
}tMDBcmdARB;
tMDBcmdARB LastMDBcmdARB=0;

uint8_t CommandLevel;       //CommandLevel van de CC machine , deze moet 3 zijn
bool SetupDoneCC;    // Geeft aan of er een setup is gedaan
bool ResetNeededCC;    // Geeft aan of er gereset moet worden voor future use.
bool CCAcceptingCoins=false;  // Geeft aan of de CC ge Enabeled of ge Disabled is
uint8_t CC_tebetalen=0;
int16_t CC_tebetalen_geheugen=0;
bool CC_geld_innemen=false;
bool RequestTubeStatus=false;// Geeft aan dat er een Tube Status moet komen
bool MDBTransOke;
char MDB_RxBuffer[60];
uint8_t MDB_RxLen;
UART_select_t MDB_COM;

MDB_Error_t MDB_Error = {
  .TubeEmpty = 1,
  .CCdefect = 1,
  .Escrow = 1,
  .WrongCmdLevel = 1,
  .Reserve = 4
};

MDB_Warning_t MDB_Warning = {
	.TubeFull = 1,
	.TubeAlmostEmpty = 1,
	.CoinRouting = 1,
	.CoinReject = 1,
	.MesageError = 1,
	.PayoutFail = 1,
	.DecodeTimeOut = 1,
	.Reserve = 1,
};

////////////////////////////////////////////////////////////////////////
void MDB_init(UART_select_t Com);
void MDB_New_data(char c);
uint8_t high_byte(uint16_t byte);
uint8_t low_byte(uint16_t byte);

bool MDB_Send_data(char * data, uint8_t lenght);
bool MDB_Read_data(char * data, uint8_t * lenght);
bool GetStatusCC(void);
bool ResetCC(void);
bool GetCC_setup_state(void);
bool GetCC_tube_status(void);
bool CCchange(unsigned int hoeveel, unsigned int type);

bool ED_CC(unsigned long COINenable , unsigned long ManualDE);
bool L3_Payout(unsigned int data);
bool RequestPayedCoins(void);
bool L3_Payout_value_poll(void);
void MDB_TimeOut(void);
void LeesTubeStatus(void);
void MDB_Poll(void);

void SerDecodeMDB(void);
////////////////////////////////////////////////////////////////////////
PROCESS(mdb_process, "mdb process");

PROCESS_THREAD(mdb_process, ev, data)
{
  PROCESS_BEGIN();
  etimer_set(&tmMDB_Decode,CLOCK_SECOND/2);
  etimer_set(&tmMDB_poll,CLOCK_SECOND+1);
  while(1){
	  if(ev == PROCESS_EVENT_TIMER){
		  if(etimer_expired(&tmMDB_Decode)){
			  SerDecodeMDB();
			  etimer_set(&tmMDB_Decode,5);
		  }
		  if(etimer_expired(&tmMDB_timeout)){
			  MDB_TimeOut();
		  }
		  if(etimer_expired(&tmMDB_poll)){
			  MDB_Poll();
			  LeesTubeStatus();
			  etimer_set(&tmMDB_poll,CLOCK_SECOND/10);
		  }

		  if(etimer_expired(&tmMDB_event_poll)){
			  if(CCgeldIn > 0){
				  process_post(PROCESS_BROADCAST,event_coin_inserted,&CCgeldIn);
				  etimer_set(&tmMDB_event_poll,CLOCK_SECOND *2);
			  }
		  }
  	  }
	  PROCESS_WAIT_EVENT();
  }
  PROCESS_END();
}



////////////////////////////////////////////////////////////////////////


uint8_t high_byte(uint16_t byte){
	return (byte > 8) & 0xFF;
}

uint8_t low_byte(uint16_t byte){
	return (byte & 0xFF );
}

void MDB_New_data(char c){
	if(MDB_RxLen  == MDB_BUFFER_LENGHT)
	{
		printf("MDB: buffer full\r\n");
		return;
	}
	MDB_RxBuffer[MDB_RxLen] = c;
	MDB_RxLen++;
	return;
}

void MDB_init(UART_select_t Com){
	UART_config_type config;
	g_Devices_available.Coin_changer_available = ini_getbool("Device","Coin_changer_available",false,"Coin available:",inifile_device);
	if(g_Devices_available.Coin_changer_available == false){
		return;
	}
	config.settings.Baud_rate = 9600;
	config.settings.Databits = UART_DATABIT_8;
	config.settings.Parity = UART_PARITY_NONE;
	config.settings.Stopbits = UART_STOPBIT_1;
	config.New_data = MDB_New_data;
	config.Auto_CTS = false;
	config.Auto_RTS = false;

	MDB_COM = Com;
	UART_poort_Init(Com, config);
	//Claim a event numbers
	event_coin_inserted = process_alloc_event();
	event_coin_payed = process_alloc_event();
	event_MDB_state = process_alloc_event();
	process_start(&mdb_process,NULL);
	MDB_Error.CCdefect = false;
	MDB_Error.Escrow = false;
	MDB_Error.TubeEmpty = false;
	return;
}


bool MDB_Send_data(char * data, uint8_t lenght){
  uint32_t Bytes_send = 0 ;
  Bytes_send =  UART_send(MDB_COM, (uint8_t *)data,lenght);
  if(lenght != Bytes_send){
	  printf("MDB: MDB timeout\r\n");
	  return false;
  }
  return true;
}

////////////////////////////////////////////////////////////////////////
bool GetStatusCC(void)
{ //stuur cmd 0x01
  char TxBuffer;
  TxBuffer=1;
  return MDB_Send_data(&TxBuffer,1);
}

////////////////////////////////////////////////////////////////////////
bool ResetCC(void)
{ //stuur cmd 0x0A
  char TxBuffer;
  TxBuffer=10;
  return MDB_Send_data(&TxBuffer,1);
}

////////////////////////////////////////////////////////////////////////
bool GetCC_setup_state(void)
{ //stuur cmd 0x0B
  char TxBuffer;
  TxBuffer=11;
  return MDB_Send_data(&TxBuffer,1);
}

////////////////////////////////////////////////////////////////////////
bool GetCC_tube_status(void)
{ //stuur cmd 0x0D
  char TxBuffer;
  TxBuffer=13;
  return MDB_Send_data(&TxBuffer,1);
}

////////////////////////////////////////////////////////////////////////
bool CCchange(unsigned int hoeveel, unsigned int type)
{ //stuur cmd 0x0E 0bxxxxyyyy CKS
  char TxBuffer[3];
  TxBuffer[0]=14;
  TxBuffer[1]=hoeveel<<(4+type);
  TxBuffer[2]=TxBuffer[1];
  return MDB_Send_data(TxBuffer,3);
}

////////////////////////////////////////////////////////////////////////
bool ED_CC(unsigned long COINenable , unsigned long ManualDE)
{ //stuur cmd 0x0C COINenable ManualDE
  char TxBuffer[6];
  TxBuffer[0]=12;
  TxBuffer[1]= high_byte(COINenable);
  TxBuffer[2]= low_byte(COINenable);
  TxBuffer[3]= high_byte(ManualDE);
  TxBuffer[4]= low_byte(ManualDE);
  TxBuffer[5]=(TxBuffer[1]+TxBuffer[2]+TxBuffer[3]+TxBuffer[4]);
  return MDB_Send_data(TxBuffer,6);
}

////////////////////////////////////////////////////////////////////////
bool L3_Payout(unsigned int data)
{ //stuur cmd 0x 0F 02 xx
  char TxBuffer[3];
  TxBuffer[0]=15;
  TxBuffer[1]=2;
  TxBuffer[2]=data;
  return MDB_Send_data(TxBuffer,3);
}

////////////////////////////////////////////////////////////////////////
bool RequestPayedCoins(void)
{ //stuur cmd 0x 0F 03
  char TxBuffer[2];
  TxBuffer[0]=15;
  TxBuffer[1]=3;
  return MDB_Send_data(TxBuffer,2);
}

////////////////////////////////////////////////////////////////////////
bool L3_Payout_value_poll(void)
{ //stuur cmd 0x 0F 04
  char TxBuffer[2];
  TxBuffer[0]=15;
  TxBuffer[1]=4;
  return MDB_Send_data(TxBuffer,2);
}


////////////////////////////////////////////////////////////////////////
void MDB_TimeOut(void)
{ if(LastMDBcmdARB!=MDB_None){  //MDB respons time out error
	//printf("MDB: Timeout\r\n");
    SetupDoneCC = false;
    MDB_Warning.DecodeTimeOut=1;
    MDB_RxLen=0;
    LastMDBcmdARB=0;
  }
}

////////////////////////////////////////////////////////////////////////
void LeesTubeStatus(void)
{
   if(LastMDBcmdARB==MDB_None){ // check of ik nog steeds moet wachten
    MDBTransOke=GetCC_tube_status( );
    LastMDBcmdARB=MDB_GetTubeLevel;
    etimer_set(&tmMDB_timeout,MDB_time_out);
    printf("MDB: read tube state\r\n");
   } else {
    RequestTubeStatus=true; //zo snel mogelijk weer
  }
}

////////////////////////////////////////////////////////////////////////
void MDB_Poll(void)
{ if(LastMDBcmdARB==MDB_None){ // check of ik nog steeds moet wachten
    LastMDBcmdARB=MDB_PollStatus;
    MDBTransOke=GetStatusCC( );
    etimer_set(&tmMDB_timeout,MDB_time_out);
  }
}

////////////////////////////////////////////////////////////////////////
bool MDB_Betaal(signed long geld)
{ if((geld>=0) && (geld<=(255*CoinSF))){
	MDB_Warning.PayoutFail = false;
    CC_tebetalen=low_byte(geld/CoinSF);
    if(SetupDoneCC){
      if(LastMDBcmdARB==0){ // check of ik nog steeds moet wachten
        if(CommandLevel==3){ //level check
          printf("MDB: paying %d\r\n",geld);
          MDBTransOke=L3_Payout(CC_tebetalen );
          MDB_Error.WrongCmdLevel=false;
          LastMDBcmdARB=MDB_PayOut;
          etimer_set(&tmMDB_timeout,MDB_time_out);
          CC_tebetalen_geheugen=(signed long) (CC_tebetalen)*CoinSF;
          CC_tebetalen=0;
         }
        else {// level 2 dus
          MDB_Error.WrongCmdLevel=true;
          printf("MDB: wrong cmd level\r\n");
        }
      }
    }
    return true;
  }
  return false;
}

////////////////////////////////////////////////////////////////////////
void MDB_Ontvang(bool ontvang)
{ CC_geld_innemen=ontvang;
  if(SetupDoneCC){
    if(LastMDBcmdARB==0){ // check of ik nog steeds moet wachten
      if(ontvang){ // CC zet aan
    	MDBTransOke=ED_CC((TubesPresent|0x38),0xFFFF ); // inname munten aangezet, manual ook aan.
        LastMDBcmdARB=MDB_AcceptCoins;
        etimer_set(&tmMDB_timeout,MDB_time_out);
        printf("MDB: inhibit off\r\n");
       }
      else{ // CC zet uit ( manual willen we behouden)
    	MDBTransOke=ED_CC(0,0xFFFF ); // inname munten uitgeschakeld, echter uitgave via manual knoppen nog ingeschakeld
        LastMDBcmdARB=MDB_AcceptCoins;
        etimer_set(&tmMDB_timeout,MDB_time_out);
        printf("MDB: inhibit on\r\n");
      }  
    }
  }
}







////////////////////////////////////////////////////////////////////////
void SerDecodeMDB(void){

char RxWerk;
uint8_t Cnt,CKS, Tube;
  if(MDB_RxLen > 0){ // Is er data?
    switch(LastMDBcmdARB){
      case MDB_PollStatus:{ //Alle errors staan "erin"

        switch(MDB_RxBuffer[0]){
          case 0x50: //CC enabled
          case 0x70: //CC&BA enabled
          case 0x72: {//CC ena, BA dis
            if(ResetNeededCC){ // check of reset is geweest? na reset namelijk geen disabled status van CC
              CCAcceptingCoins=true;  //niks aan de hand
            }
            MDB_RxLen=0;
            LastMDBcmdARB=MDB_None;
            etimer_set(&tmMDB_timeout,CLOCK_SECOND*10);
            break;}
          case 0x51: //CC disabled
          case 0x71: //CC dis, BA ena
          case 0x73: {//CC&BA disabled
            CCAcceptingCoins=false; //CC is uit!
            MDB_RxLen=0;
            LastMDBcmdARB=MDB_None;
            etimer_set(&tmMDB_timeout,CLOCK_SECOND*10);
            SetupDoneCC=false;
            break;}
          case 0x40: // niks aangesloten
          case 0x60: //BA enabled,(geen CC)
          case 0x62:{ //BA disabled (geen CC)
            SetupDoneCC=false;
            CCAcceptingCoins=false; //CC is uit!
            MDB_RxLen=0;
            LastMDBcmdARB=MDB_None;
            etimer_set(&tmMDB_timeout,CLOCK_SECOND*10);
            break;}
          case 0x90: //CC data
          case 0x91:{ //CC data, er is een BA actief
            if(MDB_RxLen>1){ // data binnen?
              if(MDB_RxLen>(MDB_RxBuffer[1]+1)){ // genoeg data bytes?
                CKS=0;
                for(Cnt=0;Cnt<=(MDB_RxBuffer[1]);Cnt++){//maak checksum
                  CKS+=MDB_RxBuffer[Cnt];
                }
                if(CKS==(MDB_RxBuffer[(MDB_RxBuffer[1]+1)])){
                  for(Cnt=1;Cnt<MDB_RxBuffer[1];Cnt++){
                    RxWerk=MDB_RxBuffer[Cnt+1];
                    switch( (RxWerk & 0b11000000)){// eerste 2 bits
                      case 0xC0:
                      case 0x80:{ // muntjes handmatig uitgespuugd door machine
                        if(CoinTube[(RxWerk & 0b00000111)] > MDB_RxBuffer[Cnt+2]){
                          CoinTube[(RxWerk & 0b00000111)] -= MDB_RxBuffer[Cnt+2];
                         }
                        else{
                          CoinTube[(RxWerk & 0b00000111)] = 0;
                        }                        
                        Cnt++; // sla data byte over, dit bevat max tube inhoud
                        break;}
                      case 0x40:{ // muntje ontvangen
                        switch(RxWerk & 0b00110000){
                          case 0: // cashbox
                            //CoinAantal[RxWerk & 0b00000111]+=1;
                            Cash_Box+=(CoinValue[RxWerk & 0b00000111]*CoinSF)/5;//altijd in 5centen rekenen
                            CCgeldIn+=CoinValue[RxWerk & 0b00000111]*CoinSF;
                            process_post(PROCESS_BROADCAST,event_coin_inserted,&CCgeldIn);
                            printf("MDB: received %d\r\n",CCgeldIn);
                            etimer_set(&tmMDB_event_poll,CLOCK_SECOND *1);
                            break;
                          case 0x10: // tube
                            //CoinAantal[RxWerk & 0b00000111]+=1;
                            CoinTube[RxWerk & 0b00000111]=MDB_RxBuffer[Cnt+2];
                            CCgeldIn+=CoinValue[RxWerk & 0b00000111]*CoinSF;
                            process_post(PROCESS_BROADCAST,event_coin_inserted,&CCgeldIn);
                            printf("MDB: received %d\r\n",CCgeldIn);
                            etimer_set(&tmMDB_event_poll,CLOCK_SECOND *1);
                            break;
                          case 0x20: // error/not_used
                            MDB_Warning.MesageError=true;
                            printf("MDB: Message error\r\n");

                            break;
                          case 0x30: // reject
                            MDB_Warning.CoinReject=true;
                            printf("MDB: coin reject\r\n");
                            break;
                        }
                        Cnt++; // sla data byte over, dit bevat max tube inhoud
                      break;}
                    }
                    switch(RxWerk){
                      case 1:    //Escrow request (hendel)
                        MDB_Error.Escrow=true;
                        printf("MDB: Escrow\r\n");
                        break;
                      case 2:    // CC busy payout
                        break;
                      case 3:    // No credit
                        break;
                      case 4:    // Defect in tube sensor
                        MDB_Error.CCdefect=true;
                        printf("MDB: defect sensor\r\n");
                        break;
                      case 5:    // Double arrival (te snel munten "ontvangen", niet geaccepteert)
                        break;
                      case 6:    // Acceptor is weg
                        MDB_Error.CCdefect=true;
                        printf("MDB: Acceptor gone \r\n");
                        break;
                      case 7:    // Tube JAM
                        MDB_Error.CCdefect=true;
                        printf("MDB: Tube jam \r\n");
                        break;
                      case 8:    // ROM CKS error
                        MDB_Error.CCdefect=true;
                        printf("MDB: ROM CKS error \r\n");
                        break;
                      case 9:    // Coin routing error
                        MDB_Warning.CoinRouting=true;
                        printf("MDB: Coin routing error \r\n");
                        break;
                      case 10:  // CC busy
                        break;
                      case 11:  // CC was reset
                        break;
                      case 12:  // Coin JAM
                        MDB_Error.CCdefect=true;
                        printf("MDB: Coin JAM \r\n");
                        break;
                    }
                  }
                 }
                else { // checksum error
                  MDB_Warning.MesageError=true;
                  printf("MDB: checksum error \r\n");
                }
                MDB_RxLen=0;
                LastMDBcmdARB=0;
                etimer_set(&tmMDB_timeout,CLOCK_SECOND*10);
              }
              //data nog niet binnen doen dan nop (wacht op volgende check op data)
            }
            break;}
          default:{  // ander soort bericht warning dus
            MDB_Warning.MesageError=true;
            printf("MDB: unknown message \r\n");
            MDB_RxLen=0;
            LastMDBcmdARB=0;
            break;}
        }
        break;}
        
      case MDB_ResetCC: {
        if(MDB_RxBuffer[0]!=0){
          MDB_Warning.MesageError=true;
          MDBTransOke=ResetCC( );
          etimer_set(&tmMDB_timeout,MDB_time_out);
         }
        else{
          ResetNeededCC=true;
          LastMDBcmdARB=0;
          etimer_set(&tmMDB_timeout,CLOCK_SECOND*10);
        }
        MDB_RxLen=0;
        break;}
        
      case MDB_ReadSetting:{
        if(MDB_RxLen>23){
          if(MDB_RxBuffer[0]==24){//check op geldig bericht.
            CommandLevel=MDB_RxBuffer[1];
            CoinSF=MDB_RxBuffer[4];
            TubesPresent=MDB_RxBuffer[7];     //TubesPresent is eigenlijk 2bytes maar we gebruiken niet 16 tubes.
            memcpy(CoinValue,&MDB_RxBuffer[8],7);
            SetupDoneCC=true;
            // verzenden van aanzet bericht om handmatig de munten uittespugen            
            MDBTransOke=ED_CC(0,0xFFFF ); // inname munten uitgeschakeld, echter uitgave via manual knoppen nog ingeschakeld
            LastMDBcmdARB=MDB_AcceptCoins;
            etimer_set(&tmMDB_timeout,MDB_time_out);
            CCAcceptingCoins=true;
           }
          else {// setup status error
            MDB_Warning.MesageError=true;
            printf("MDB: setup status error \r\n");
            SetupDoneCC=false;
            MDBTransOke=GetCC_setup_state( ); // nieuwe setup request
            etimer_set(&tmMDB_timeout,MDB_time_out);
          }
          MDB_RxLen=0;
        }
        break;}
        
      case MDB_GetTubeLevel: {
        if(MDB_RxLen>MDB_RxBuffer[0]){ //genoeg data?
          if(MDB_RxBuffer[2]!=0){ // er is een tube vol!
            for(Cnt=0;Cnt<7;Cnt++){
              if((MDB_RxBuffer[2]>>Cnt) & 0b00000001){
                if(MDB_RxBuffer[Cnt+3]){ //check of niet 0
                  CoinTubeMAX[Cnt]=MAX(CoinTubeMAX[Cnt],MDB_RxBuffer[Cnt+3]);
                }
              }
            }  
            MDB_Warning.TubeFull=true;
            //printf("MDB: Tube Full \r\n");
          }
          memcpy(CoinTube,&MDB_RxBuffer[3],7);//copieer alle waarden naar van tube naar log
          MDB_Warning.TubeAlmostEmpty=false;// clearen
          MDB_Error.TubeEmpty=false;// clearen
          for(Tube=0;Tube<7;Tube++){ // controleer of een tube bijna leeg is
            if((TubesPresent>>Tube)&0b00000001){
              if(CoinTube[Tube]<6){         //Alarm bij minder dan 8 per buis
                MDB_Warning.TubeAlmostEmpty=true;// warning Tube Dangerously low
              }
              if(CoinTube[Tube]==0){
                MDB_Error.TubeEmpty=true;
              }
            }
          }
          LastMDBcmdARB=0;
          MDB_RxLen=0;
          etimer_set(&tmMDB_timeout,CLOCK_SECOND*10);
        }
        break;}
        
      case MDB_PayCoins: {
        if(MDB_RxBuffer[0]!=0){
          MDB_Warning.MesageError=true;
          LastMDBcmdARB=0;
          etimer_set(&tmMDB_timeout,CLOCK_SECOND*10);
         }
        else{
          if(CommandLevel==3){ // check of er gekeken moet worden wat er gespuugd is. Let op het is de verantwoordelijkheid van de VMC om bij te houden wat er in de aantallen zit wanneer level=2
            MDBTransOke=RequestPayedCoins( );
            MDB_Error.WrongCmdLevel=false;
            LastMDBcmdARB=MDB_PayedCoins;
            etimer_set(&tmMDB_timeout,MDB_time_out);
           }
          else {
            MDB_Error.WrongCmdLevel=true;
            printf("MDB: Wrong command level \r\n");
            LastMDBcmdARB=0;
            etimer_set(&tmMDB_timeout,CLOCK_SECOND*10);
          }
        }        
        MDB_RxLen=0;
        break;}
        
      case MDB_AcceptCoins: {
        if(MDB_RxBuffer[0]!=0){
          MDB_Warning.MesageError=true;
          printf("MDB: Accept Coins error \r\n");
         }
        else {
          ResetNeededCC=false;
          CCAcceptingCoins = !CCAcceptingCoins; 
        }
        LastMDBcmdARB=0;
        MDB_RxLen=0;
        etimer_set(&tmMDB_timeout,CLOCK_SECOND*10);
        break;}
        
      case MDB_PayOut: {
        if(MDB_RxBuffer[0]!=0){
          MDB_Warning.MesageError=true;
          printf("MDB: Payout error \r\n");
          LastMDBcmdARB=0;
          etimer_set(&tmMDB_timeout,CLOCK_SECOND*10);
         }
        else {
          if(CommandLevel==3){ // check of er gekeken moet worden wat er gespuugd is. Let op het is de verantwoordelijkheid van de VMC om bij te houden wat er in de aantallen zit wanneer level=2
            MDBTransOke=RequestPayedCoins( );
            MDB_Error.WrongCmdLevel=false;
            LastMDBcmdARB=MDB_PayedCoins;
            etimer_set(&tmMDB_timeout,MDB_time_out);
           }
          else{  // error
            MDB_Error.WrongCmdLevel=true;
            printf("MDB: Wrong command level \r\n");
            LastMDBcmdARB=0;
            etimer_set(&tmMDB_timeout,CLOCK_SECOND*10);
          }
        }
        MDB_RxLen=0;
        break;}  
        
      case MDB_PayedCoins: {
        if(MDB_RxLen>MDB_RxBuffer[0]){// genoeg data? anders wachten daarop. Er moet nog een time-out timer komen
          MDB_RxLen=0;
          if(MDB_RxBuffer[0]==1){// te snel na een payout status aangeroepen!!
            //stuur een nieuwe request voor munten gespuugd..
            MDBTransOke=RequestPayedCoins( );
            LastMDBcmdARB=MDB_PayedCoins;
            etimer_set(&tmMDB_timeout,MDB_time_out);
           }else{
            int Tube;
            for(Tube=0;Tube<7;Tube++){
              if(CoinTube[Tube]>MDB_RxBuffer[Tube+1]){
                CoinTube[Tube]-=MDB_RxBuffer[Tube+1];
              }
              CCgeldUit+=(signed long) (CoinValue[Tube]) * (MDB_RxBuffer[Tube+1]) * CoinSF;
            }
            if(CC_tebetalen_geheugen!=CCgeldUit){ // check of er correct uitgegooid is
              MDB_Warning.PayoutFail=true;
              printf("MDB: payout failed \r\n");
              // wat met het resulterende te betalen? kan uitgezet worden met MDB_Betaal(0);
            }          
            LastMDBcmdARB=0;
            printf("MDB: event \r\n");
            process_post(PROCESS_BROADCAST,event_coin_payed,&CCgeldUit);
            etimer_set(&tmMDB_timeout,CLOCK_SECOND*10);
          }
        }
        break;}
      default:
    	  break;
    }
  }
  
  
  ///// na beraad is besloten om gelijk na een decode nieuwe dingen te sturen behalve de poll
  if(LastMDBcmdARB==MDB_None){ // check of ik nog steeds moet wachten
    if(!SetupDoneCC){// check of setup al een keer is geweest
      LastMDBcmdARB=MDB_ReadSetting;
      MDBTransOke=GetCC_setup_state( );
      etimer_set(&tmMDB_timeout,MDB_time_out);
     }
    else if(CC_tebetalen>0){ // check of ik muntjes moet uitbetalen
      if(CommandLevel==3){ //level check
        MDBTransOke=L3_Payout(CC_tebetalen );
        MDB_Error.WrongCmdLevel=false;
        LastMDBcmdARB=MDB_PayOut;
        etimer_set(&tmMDB_timeout,MDB_time_out);
        CC_tebetalen_geheugen=(signed long) (CC_tebetalen)*CoinSF;
        CC_tebetalen=0;
       }
      else{ // level 2 dus
        MDB_Error.WrongCmdLevel=true;
      }
     }
    else if(CC_geld_innemen && !CCAcceptingCoins){ // ik mot geld krijgen
      MDBTransOke=ED_CC((TubesPresent|0x38),0xFFFF ); // inname munten aangezet, manual ook aan.
      LastMDBcmdARB=MDB_AcceptCoins;
      etimer_set(&tmMDB_timeout,MDB_time_out);
     }
    else if(!CC_geld_innemen && CCAcceptingCoins){ //genoeg betaald dus uitzetten
      // manual willen we behouden, wordt dus berekent
      MDBTransOke=ED_CC(0,0xFFFF ); // inname munten uitgeschakeld, echter uitgave via manual knoppen nog ingeschakeld
      LastMDBcmdARB=MDB_AcceptCoins;
      etimer_set(&tmMDB_timeout,MDB_time_out);
     }
    else if(RequestTubeStatus){
      MDBTransOke=GetCC_tube_status( );
      LastMDBcmdARB=MDB_GetTubeLevel;
      etimer_set(&tmMDB_timeout,MDB_time_out);
      RequestTubeStatus=false;
    }
  }
}


