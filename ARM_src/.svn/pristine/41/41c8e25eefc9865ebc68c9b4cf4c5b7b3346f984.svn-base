#include <core/Timer/etimer.h>
#include <core/protocol/Secure/sha1/sha1.h>
#include <core/Debug/Debug_Terminal.h>
#include <core/Thread/pt.h>
#include <Device/I2C/RFID/RFID.h>
#include <Device/I2C/RFID/RFID_Pass.h>
#include <Core/Clock/Time_functions.h>
#include <Core/protocol/Net/SCP/SCP_Comm.h>
#include <Core/protocol/Secure/aes/aes.h>
#include <Core/protocol/Secure/sha1/sha1.h>
#include <Core/protocol/Checksum/crc16/crc16_CCITT.h>
#include <Core/Extensions/code/access_code.h>

struct pt RFID_pass_pt;
struct pt RFID_sub_pt;

const char RFID_SALT_UNLOCK[7] = {0xf8,0x0f,0x81,0xa5,0x20,0x49,0xf2};
const char RFID_SALT_CREDIT[7] = {0xaa,0x9c,0x0a,0x03,0x63,0x62,0x68};

void RFID_calc_unlock_code(RFID_reader_t * reader, uint8_t * hash){
	SHA1Context Pass_SHA;
	uint8_t location;
	uint32_t refnr;
	SHA1Init(&Pass_SHA);

	SHA1Update(&Pass_SHA, reader->reader_card.card_UID.UID, 7);
	SHA1Update(&Pass_SHA, &reader->reader_card.card_info.auth_lev, 1);
	if(reader->reader_card.card_info.card_version == 1){
		location = SCP_Get_device_info().SCP_locnr;
		if(reader->reader_card.card_info.auth_lev < AUTH_SERVICE){
			SHA1Update(&Pass_SHA, &location, 1);
		}
	}
	SHA1Update(&Pass_SHA, RFID_SALT_UNLOCK, 7);
	refnr = Load_code_from_sd(256);
	if((refnr != 0)&&(reader->reader_card.card_info.auth_lev < AUTH_MANUFACTURE)){
		SHA1Update(&Pass_SHA,&refnr, 4);
	}
	SHA1Final(&Pass_SHA, (void *)hash);
	return;
}

PT_THREAD(_RFID_Set_Unlock_Code(RFID_reader_t * reader, process_event_t ev, process_data_t data))
{
	static Status com_status;
	uint8_t hash[SHA1_HASH_SIZE];

	PT_BEGIN(&RFID_pass_pt);

	if(reader->reader_card.card_info.availible == false){
		PT_EXIT(&RFID_pass_pt);
	}

	RFID_calc_unlock_code(reader,hash);

	PT_SPAWN(&RFID_pass_pt,&pt_RFID_send_data,RFID_Write_Data(reader, &com_status, UNLOCK_CODE_BLOCK ,KeyA ,0 ,(char *)hash));
	printf("RFID: write lock ");
	if(com_status == SUCCESS){
		reader->reader_card.card_action.Unlock_code_write = false;
		printf("OK\r\n");
	}else{
		printf("FAILED!\r\n");
	}
	PT_END(&RFID_pass_pt);
}

PT_THREAD(_RFID_Get_Unlock_Code(RFID_reader_t * reader, process_event_t ev, process_data_t data))
{
	static Status com_status;
	char card_data[16];

	PT_BEGIN(&RFID_pass_pt);

	if(reader->reader_card.card_info.availible == false){
		PT_EXIT(&RFID_pass_pt);
	}

	PT_SPAWN(&RFID_pass_pt,&pt_RFID_send_data,RFID_Read_Data(reader, &com_status, UNLOCK_CODE_BLOCK,KeyA,0,card_data));
	if(com_status == ERROR){
		PT_EXIT(&RFID_pass_pt);
	}

	uint8_t hash[SHA1_HASH_SIZE];
	RFID_calc_unlock_code(reader, hash);

	if(memcmp(hash,card_data,16) == 0){
		reader->reader_card.card_unlock_code.ok = true;
	}else{
		reader->reader_card.card_unlock_code.ok = false;
	}

	printf("RFID: unlock code %s\r\n",reader->reader_card.card_unlock_code.ok == true?"OK":"FAILED");
	reader->reader_card.card_unlock_code.availible = true;

	PT_END(&RFID_pass_pt);
}

PT_THREAD(_RFID_Set_Info(RFID_reader_t * reader, process_event_t ev, process_data_t data))
{
	static Status com_status;
	char card_data[16];
	uint8_t i;
	time_t Start_tm, End_tm;

	PT_BEGIN(&RFID_pass_pt);

	if(reader->reader_card.card_info.availible == false){
		PT_EXIT(&RFID_pass_pt);
	}

	card_data[LANGUAGE_BYTE] = (reader->reader_card.card_info.Language + 1);
	card_data[MOD_NR_BYTE] = reader->reader_card.card_info.last_mod_nr;

	printf ( "RFID: Write Start date: %s ", asctime (LocalTime(&reader->reader_card.card_info.Start_date)) );
	printf ( "RFID: Write End date %s\r\n", asctime (LocalTime(&reader->reader_card.card_info.End_date)) );
	Start_tm = mktime(&reader->reader_card.card_info.Start_date);
	End_tm = mktime(&reader->reader_card.card_info.End_date);
	memset(&card_data[START_END_DATE],0,8);

	memmove(&card_data[START_END_DATE],&Start_tm,4);
	memmove(&card_data[START_END_DATE+4],&End_tm,4);
	card_data[START_END_DATE+4]= 0;
	card_data[AUTH_LEV_BYTE] = reader->reader_card.card_info.auth_lev + 1;
	card_data[CARD_VERSION_BYTE] = CURRENT_CARD_V;

	card_data[15] = 1;
	for(i=0;i<15;i++){
		card_data[15] ^= card_data[i];
	}

	PT_SPAWN(&RFID_pass_pt,&pt_RFID_send_data,RFID_Write_Data(reader, &com_status, INFO_BLOCK ,KeyA ,0 ,card_data));

	printf("RFID: write info ");
	if(com_status == SUCCESS){
		reader->reader_card.card_action.Info_write = false;
		printf("OK\r\n");
	}else{
		printf("FAILED!\r\n");
	}

	PT_END(&RFID_pass_pt);
}

PT_THREAD(_RFID_Get_Info(RFID_reader_t * reader, process_event_t ev, process_data_t data))
{
	static Status com_status;
	char card_data[16];
	time_t Start_tm = 0, End_tm = 0;
	uint8_t check , i;

	PT_BEGIN(&RFID_pass_pt);

	PT_SPAWN(&RFID_pass_pt,&pt_RFID_send_data,RFID_Read_Data(reader, &com_status, INFO_BLOCK,KeyA,0,card_data));

	if(com_status == ERROR){
		PT_EXIT(&RFID_pass_pt);
	}

	check = 1;
	for(i=0;i<15;i++){
		check ^= card_data[i];
	}

	if(check != card_data[15]){
		printf("RFID: Info crc failed\r\n");
		PT_EXIT(&RFID_pass_pt);
	}

	printf ( "RFID: Start date: %s ", asctime (LocalTime(&reader->reader_card.card_info.Start_date)) );
	if(card_data[LANGUAGE_BYTE] == 0){
		reader->reader_card.card_info.Language = NOT_SET;
	}else if(card_data[LANGUAGE_BYTE] <= NOT_SET){
		reader->reader_card.card_info.Language = (card_data[LANGUAGE_BYTE] - 1);
	}else{
		reader->reader_card.card_info.Language = NOT_SET;
	}

	reader->reader_card.card_info.last_mod_nr = card_data[MOD_NR_BYTE];
	reader->reader_card.card_info.card_version = card_data[CARD_VERSION_BYTE];


	memmove(&Start_tm,&card_data[START_END_DATE],4);
	memmove(&End_tm,&card_data[START_END_DATE+4],4);

	reader->reader_card.card_info.Start_date = *gmtime(&Start_tm);
	reader->reader_card.card_info.End_date = *gmtime(&End_tm);
	reader->reader_card.card_info.Date_ok = CurTime_in_periode(reader->reader_card.card_info.Start_date, reader->reader_card.card_info.End_date, false) == 0 ? true:false;

	if(card_data[AUTH_LEV_BYTE] == 0){
		reader->reader_card.card_info.auth_lev = 0;
		printf ( "Auth: no\r\n");
	}else{
		reader->reader_card.card_info.auth_lev = card_data[AUTH_LEV_BYTE] - 1;
	}

	printf ( "RFID: End date: %s ", asctime (LocalTime(&reader->reader_card.card_info.End_date)) );
	printf ( "Auth: %d\r\n", reader->reader_card.card_info.auth_lev);

	reader->reader_card.card_info.availible = true;

	PT_END(&RFID_pass_pt);
}

PT_THREAD(_RFID_Set_Credit(RFID_reader_t * reader, process_event_t ev, process_data_t data))
{
	static Status com_status;
	SHA1Context Pass_SHA;
	uint8_t Key_hash[SHA1_HASH_SIZE];
	char card_data[16];
	char card_data_encrypt[16];
	AES_KEY SCP_AES_KEY;
	uint16_t crc = 0;

	PT_BEGIN(&RFID_pass_pt);

	if(reader->reader_card.card_info.availible == false){
		PT_EXIT(&RFID_pass_pt);
	}

	if(reader->reader_card.card_credit.availible == false){
		PT_EXIT(&RFID_pass_pt);
	}

	SHA1Init(&Pass_SHA);
	SHA1Update(&Pass_SHA, reader->reader_card.card_UID.UID, 7);
	SHA1Update(&Pass_SHA, &reader->reader_card.card_info.auth_lev, 1);
	SHA1Update(&Pass_SHA, RFID_SALT_CREDIT, 7);
	uint32_t refnr = Load_code_from_sd(256);
	if((refnr != 0)&&(reader->reader_card.card_info.auth_lev < AUTH_MANUFACTURE)){
		SHA1Update(&Pass_SHA,&refnr, 4);
	}
	SHA1Final(&Pass_SHA, Key_hash);

	memset(card_data,0,16);
	memcpy(card_data,&reader->reader_card.card_credit.Credit, 4);
	card_data[USER_NR_BYTE] = reader->reader_card.card_credit.User_nr;
	card_data[TRANS_NR_BYTE] = reader->reader_card.card_credit.Transaction_nr++;
	crc = crc16ccitt_xmodem_remainder((uint8_t *)card_data, 14, crc);
	memcpy(&card_data[14],&crc, 2);

	AES_set_encrypt_key((unsigned char *)Key_hash,128,&SCP_AES_KEY);
	AES_ecb_encrypt( (unsigned char *)card_data, (unsigned char *)card_data_encrypt ,&SCP_AES_KEY, AES_ENCRYPT);

	PT_SPAWN(&RFID_pass_pt,&pt_RFID_send_data,RFID_Write_Data(reader,&com_status, CREDIT_BLOCK ,KeyA ,0 ,card_data_encrypt));

	printf("RFID: write credit ");
	if(com_status == SUCCESS){
		reader->reader_card.card_action.Credit_write = false;
		printf("OK\r\n");
	}else{
		printf("FAILED!\r\n");
	}

	PT_END(&RFID_pass_pt);
}

PT_THREAD(_RFID_Get_Credit(RFID_reader_t * reader, process_event_t ev, process_data_t data))
{
	static Status com_status;
	SHA1Context Pass_SHA;
	uint8_t Key_hash[SHA1_HASH_SIZE];
	char card_data[16];
	char card_data_encrypt[16];
	AES_KEY SCP_AES_KEY;
	uint16_t crc = 0;
	uint16_t crc_card = 0;

	PT_BEGIN(&RFID_pass_pt);

	if(reader->reader_card.card_info.availible == false){
		PT_EXIT(&RFID_pass_pt);
	}

	PT_SPAWN(&RFID_pass_pt,&pt_RFID_send_data,RFID_Read_Data(reader, &com_status, CREDIT_BLOCK ,KeyA ,0 ,card_data_encrypt));

	SHA1Init(&Pass_SHA);
	SHA1Update(&Pass_SHA, reader->reader_card.card_UID.UID, 7);
	SHA1Update(&Pass_SHA, &reader->reader_card.card_info.auth_lev, 1);
	SHA1Update(&Pass_SHA, RFID_SALT_CREDIT, 7);
	uint32_t refnr = Load_code_from_sd(256);
	if((refnr != 0)&&(reader->reader_card.card_info.auth_lev < AUTH_MANUFACTURE)){
		SHA1Update(&Pass_SHA,&refnr, 4);
	}
	SHA1Final(&Pass_SHA, Key_hash);

	if(com_status == ERROR){
		PT_EXIT(&RFID_pass_pt);
	}

	AES_set_decrypt_key((unsigned char *)Key_hash,128,&SCP_AES_KEY);
	AES_ecb_encrypt((unsigned char *)card_data_encrypt ,(unsigned char *)card_data ,&SCP_AES_KEY, AES_DECRYPT);

	crc = crc16ccitt_xmodem_remainder((uint8_t *)card_data, 14, crc);
	memcpy(&crc_card, &card_data[14], 2);

	if(crc == crc_card){
		memcpy(&reader->reader_card.card_credit.Credit,&card_data[CREDIT_BYTE], 8);
		printf("RFID: credits %d\r\n",reader->reader_card.card_credit.Credit);
		reader->reader_card.card_credit.Transaction_nr = card_data[USER_NR_BYTE];
		reader->reader_card.card_credit.User_nr = card_data[TRANS_NR_BYTE];
		reader->reader_card.card_credit.availible = true;
	}else{
		printf("RFID: Credit crc failed\r\n");
	}

	PT_END(&RFID_pass_pt);
}
