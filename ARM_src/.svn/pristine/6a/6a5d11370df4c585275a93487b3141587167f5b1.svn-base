/**
 * \addtogroup device
 * @{
 */

/**
 * \defgroup RFID_API
 *
 * The RFID API defines a set of functions for reading and writing the RFID card.
 *
 * @{
 */

/**
 * \file
 * This file contains functions and processes for accessing the RFID card
 * \author
 *         Mark van der Heijden <m.vanderheijden@custommate.eu>
 * 
 */
  #ifndef _RFID_MicroRWD_
  #define _RFID_MicroRWD_ 
  
  #include <string.h>
  #include <stdlib.h> 
  #include <stdint.h> 
  #include <stdbool.h>
  #include <time.h>
  #include <Core/Thread/pt.h>
  #include <Core/Process/process.h>
  #include <Processes/Global_UID.h>
  #include <Core/Type/type.h>
  
  #define RFID_MAX_SPEED_HZ 400000
  
  typedef enum{
    RFID_NO_ERROR,
    RFID_I2C_ERROR,
    RFID_RS232_ERROR,
    RFID_TIMEOUT_ERROR,
    RFID_ANTENNA_ERROR,
    RFID_EEPROM_ERROR,
 }RFID_reader_error_t;

 typedef struct{
	 bool availible;
	 bool ok;
 } RFID_card_unlock_code_t;
 
 typedef struct{
	 bool availible;
	 uint8_t auth_lev;
	 Language_t Language;
	 uint8_t last_mod_nr;
	 struct tm Start_date;
	 struct tm End_date;
	 bool Date_ok;
	 uint8_t card_version;
 } RFID_card_info_t;


 typedef struct{
	 bool availible;
	 int32_t Credit;
	 uint8_t User_nr;
	 uint16_t Transaction_nr;
 } RFID_card_credit_t;

 typedef struct{
	 bool Unlock_code_read;
	 bool Unlock_code_write;
	 bool Info_read;
	 bool Info_write;
	 bool Credit_read;
	 bool Credit_write;
 } RFID_card_read_write_t;

 typedef enum{
	MIFARE_1k,
	MIFARE_4k,
	MIFARE_UL,
	HIGTAG_2,
	HIGTAG_1S,
	EM400XMC200
 } RFID_card_type_t;

 typedef struct {
	  bool card_present;
	  bool card_changed;
	  bool card_new;
	  RFID_card_type_t card_type;
	  RFID_card_read_write_t card_action;
	  UID_t card_UID;								//block 0
	  RFID_card_unlock_code_t card_unlock_code;//block 1
	  RFID_card_info_t card_info; 				//block 2
	  RFID_card_credit_t card_credit; 			//block 4
  }RFID_card_t;

 typedef enum{
	READER_MIFARE,
	READER_HIGTAG_2,
	READER_HIGTAG_1S,
	READER_EM400XMC200
 } RFID_reader_type_t;

  typedef struct {
	  uint8_t reader_addr;
	  RFID_reader_type_t reader_type;
	  RFID_reader_error_t reader_error;
	  RFID_card_t reader_card;
	  bool reader_beep_sound;
  }RFID_reader_t;
  
  typedef enum {
	 KeyA = 0,
	 KeyB = 1
  } Key_t;
  ////////////////////Global variabels/////////////////

  extern struct pt pt_RFID_send_data;
  extern process_event_t event_card_change;
  extern process_event_t event_card_busy;
  extern process_event_t event_card_done;
  extern RFID_card_t * g_RFID_card;

  #define event_card_present event_card_change && ((*(RFID_reader_t * )data).reader_card != NULL)
  #define event_card_remove event_card_change && ((*(RFID_reader_t * )data).reader_card == NULL)

  PROCESS_NAME(RFID_process);

  ///////////////////API MACRO///////////////////
  #define RFID_event_get_reader(DATA)	((RFID_reader_t *)data)
  #define RFID_event_get_card(DATA)		(((RFID_reader_t *)data)->reader_card)
  #define RFID_get_card(READER)			(&READER->reader_card)

  ///////////////////API Functions///////////////////
  

  /*
   * @brief start polling rfid reader
   * @param RFID_addr i2c address of RFID reader
   */
  RFID_reader_t * RFID_init(uint8_t RFID_addr, RFID_reader_type_t Reader_type);

  RFID_reader_t * RFID_get_reader(uint8_t RFID_addr);


  /**
  *@brief Write data to the RFID card. poll till return value is true .use RFID_get_error to check for errors.
  *@param Block_address MF Card Block Address 0 – 255
  *@param Key_type security key type type A or B(not fully implemented yet use KeyA )
  *@param Key_code_number security code(not fully implemented yet use 0)
  *@param Data Data to write to card. 16 byte
  **/
  PT_THREAD(RFID_Write_Data(RFID_reader_t * reader, Status * com_status, int Block_address ,Key_t Key_type ,int Key_code_number,char * data));
  // void RFID_Write_Data(int Block_address, Key_t Key_type, int Key_code_number, char Data[16]);
  
  /**
  *@brief Read data from the RFID card. poll till return value is true .use RFID_get_error to check for errors.
  *@param Block_address MF Card Block Address 0 – 255
  *@param Key_type security key type type A or B(not fully implemented yet use KeyA )
  *@param Key_code_number security code(not fully implemented yet use 0)
  *@param Data Pointer to received data. 16 byte
  **/
  //void RFID_Read_Data(int Block_address ,Key_t Key_type ,int Key_code_number);
  PT_THREAD(RFID_Read_Data(RFID_reader_t * reader, Status * com_status, int Block_address ,Key_t Key_type ,int Key_code_number,char * data));
  

  /**
   * @beep sound
   */
  void RFID_beep_sound(void);



#endif //_RFID_MicroRWD_
  /**
 * 	@}
 * @}
 **/
