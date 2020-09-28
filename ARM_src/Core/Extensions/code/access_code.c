#include <Board/Def_config.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Core/Extensions/ini/minIni.h>
const char * Secure_file = "SD1:/Code.cd";

#if (BOARD_SD_CARD == 1)
	void Save_code_to_sd(uint16_t auth_lev, uint32_t code){
		uint8_t encr = 0x36;
		uint8_t i;
		uint64_t encr_code;
		char key[10];

		encr_code = code;
		for(i = 0; i < 8; i++){
			encr_code ^= encr;
			encr_code = (encr_code << 1);
		}
		sprintf(key,"Key%d",auth_lev);
		ini_putl("Code",key,encr_code,"auth code:",Secure_file);

		return;
	}

	uint32_t Load_code_from_sd(uint16_t auth_lev){
		uint8_t encr = 0x36;
		uint8_t i;
		uint64_t encr_code;
		char key[10];

		sprintf(key,"Key%d",auth_lev);
		encr_code = ini_getl("Code",key,0,"auth code:",Secure_file);
		if(encr_code == 0){
			return 0;
		}
		for(i = 0; i < 8; i++){
			encr_code = (encr_code >> 1);
			encr_code ^= encr;
		}

		return encr_code;
	}

	typedef struct{
		uint16_t code;
		bool found;
		char * section;
	}Code_search_t;

	static int Code_search_on_sd(const mTCHAR *Section, const mTCHAR *Key, const mTCHAR *Value, const void *UserData){
		Code_search_t * Data = (Code_search_t *) UserData;

		if(Data->code  == atoi(Value)){
			if(strstr(Section,Data->section) != NULL){
				Data->found = true;
				return 0;
			}
		}
		return 1;
	}

	bool Access_Code_present_on_sd(uint8_t Code[7],uint8_t Code_lenght){
		Code_search_t user_data;
		uint8_t i;
		user_data.code = 0;
		for(i=0;i<Code_lenght;i++){
			user_data.code *= 10;
			user_data.code += Code[i];
		}
		user_data.section = "Access code";
		user_data.found = false;
		ini_browse(&Code_search_on_sd, &user_data, Secure_file);
		return user_data.found;
	}


#endif
