#ifndef ELTRA_CMD_H__
#define ELTRA_CMD_H__

	#include <Processes/Global_UID.h>
	#include <Core/Timer/etimer.h>
	#include <stdbool.h>
	#include <Device/I2C/RFID/RFID.h>

	typedef enum{
		READ_BARCODE_FIELD = 0x23,
		MIFARE_READ = 0x31,
		CAPTURE_TICKET = 0x3A,
		INSERT_TICKET = 0x3C,
		LED_MOUTH = 0x3E,
		GET_PHYSICAL_STATUS = 0x3F,
		EJECT_TICKET = 0x40,
		MOMORY_SETUP = 0x41,
		PICTURE_UPLOAD = 0x43,
		FONT_UPLOAD = 0x45,
		EJECT_CARD = 0x46,
		ISSUE_CARD_FROM_STK = 0x49,
		SET_THERMAL_TIME = 0x49,
		GET_THERMAL_TIME = 0x4A,
		CREATE_TICKET = 0x4C,
		ISO1443A_REQUEST_MODE = 0x61,
		ISO1443A_CASCADE_LEVEL1 = 0x62,
		RFID_TRANSPARENT_COMMAND = 0x68,
		SOFTWARE_RESET = 0x72,
		SET_PARAMETERS = 0x79,
		GET_PARAMETERS = 0x7A,
		USER_AUTHENTICATION = 0x7E,
	}ELTRA_cmd_t;

	typedef enum{
		CARD_OK,
		SYSTEM_ERROR,
		NOTAG_ERR,
		READ_ERR,
		WRITE_ERR,
		OUT_OF_RANGE
	}ELTRA_card_error_t;

	typedef enum{
		NO_ERROR,
		COMMAND_REFUSED,
		COMMAND_UNAVAILABLE,
		READ_TIMEOUT,
		TRANSPORT_ERROR,
		NO_TICKET_INSIDE,
		JAMMING,
		NO_TICKET_ON_MOUTH,
		TICKET_ON_MOUTH,
		WRONG_VALUE,
		ELTRA_TIMEOUT,
		NO_CARTRIDGE,
		SLEIGH_WRONG_POS,
		NO_MORE_CARDS
	}ELTRA_error_t;

	typedef struct{
		 ELTRA_cmd_t command;
		 ELTRA_error_t error;
		 uint8_t * data;
		 uint8_t data_len;
		 struct timer ELTRA_timeout;
	}ELTRA_event_t;

	typedef enum{
		BARCODE_LINE = 0,
		RFID_LINE = 1
	}ELTRA_line_t;

	typedef struct{
		ELTRA_line_t ELTRA_line;
		bool dispenser;
	}ELTRA_device_t;

	typedef struct {
		bool card_present;
		UID_t card_UID;
		ELTRA_card_error_t Card_status;
		RFID_card_read_write_t action;
		RFID_card_info_t info;
		bool write_ok;
		bool read_ok;
	} ELTRA_card_t;

	extern char * ELTRA_send_data;
	extern ELTRA_event_t event_data;


	uint16_t ELTRA_Data_to_ISO(uint8_t * in, uint8_t * out, uint8_t len);
	/*
	 * @brief read ticket under the scanner
	 * @param STATUS pointer to status
	 * @param DATA pointer to buffer
	 * @param DATALEN pointer to return data lenght
	 */
	#define PROCESS_ELTRA_READ_BARCODE_FIELD(STATUS, DATA ,DATALEN) 				\
		ELTRA_Data_Send(READ_BARCODE_FIELD, NULL, 0); 								\
		PROCESS_WAIT_EVENT_UNTIL(ELTRA_Check_event(ev,data,READ_BARCODE_FIELD));	\
		ELTRA_Read_Barcode_Field__Data(STATUS, DATA, DATALEN)

	/*
	 * @ brief capture ticket
	 */
	#define PROCESS_ELTRA_CAPTURE_TICKET() 					\
		ELTRA_Data_Send(CAPTURE_TICKET, NULL, 0); 			\
		PROCESS_WAIT_EVENT_UNTIL(ELTRA_Check_event(ev,data,CAPTURE_TICKET))


	uint8_t ELTRA_led_mouth(uint8_t green, uint8_t red);
	/*
	 * @brief insert ticket
	 * @param STEP number of steps to insert to position the ticket (0000 to 0300)
	 */
	#define PROCESS_ELTRA_LED_MOUTH(GREEN,RED); 											\
		ELTRA_Data_Send(LED_MOUTH, ELTRA_send_data, ELTRA_led_mouth(GREEN,RED));			\
		PROCESS_WAIT_EVENT_UNTIL(ELTRA_Check_event(ev,data,LED_MOUTH));						\
		free(ELTRA_send_data)

	uint8_t ELTRA_Insert_Ticket(uint8_t Step);
	/*
	 * @brief insert ticket
	 * @param STEP number of steps to insert to position the ticket (0000 to 0300)
	 */
	#define PROCESS_ELTRA_INSERT_TICKET(STEP); 											\
		ELTRA_Data_Send(INSERT_TICKET, ELTRA_send_data, ELTRA_Insert_Ticket(STEP));		\
		PROCESS_WAIT_EVENT_UNTIL(ELTRA_Check_event(ev,data,INSERT_TICKET));				\
		free(ELTRA_send_data)

	/*
	 * @brief get sensor info
	 */
	#define PROCESS_ELTRA_GET_PHYSICAL_STATUS() 					\
		ELTRA_Data_Send(GET_PHYSICAL_STATUS, NULL, 0); 		\
		PROCESS_WAIT_EVENT_UNTIL(ELTRA_Check_event(ev,data,GET_PHYSICAL_STATUS))


	#define MOUTH_SENSOR 0
	#define POS1_SENSOR 1
	#define POS2_SENSOR 2
	#define TAIL_SENSOR 3

	#define DISPENSER_SLIDE_SENSOR 4
	#define DISPENSER_CARTRIGE_SENSOR 5
	#define DISPENSER_HOME_SENSOR 6
	#define DISPENSER_MOUTH_SENSOR 7
	#define DISPENSER_CARDS_PRESENT_SENSOR 8
	#define DISPENSER_CARDS_LOW_SENSOR 9
	#define DISPENSER_CARDS_FULL_SENSOR 10


	/*
	 * @brief get sensor status after PROCESS_GET_PHYSICAL_STATUS
	 */
	bool ELTRA_GET_PHYSICAL_STATUS__SENSOR(uint8_t sensor);

	/*
	 * @brief eject ticket by moving it to the mouth
	 */
	#define PROCESS_ELTRA_EJECT_TICKET() 						\
		ELTRA_Data_Send(EJECT_TICKET, NULL, 0); 				\
		PROCESS_WAIT_EVENT_UNTIL(ELTRA_Check_event(ev,data,EJECT_TICKET))

	/*
	 * @brief eject ticket by moving it to the mouth
	 */
	#define PROCESS_ELTRA_EJECT_CARD() 						\
		ELTRA_Data_Send(EJECT_CARD, NULL, 0); 				\
		PROCESS_WAIT_EVENT_UNTIL(ELTRA_Check_event(ev,data,EJECT_CARD))

	/*
	 * @brief eject ticket by moving it to the mouth
	 */
	#define PROCESS_ELTRA_ISSUE_CARD_FROM_STK() 					\
		ELTRA_Data_Send(ISSUE_CARD_FROM_STK, NULL, 0); 				\
		PROCESS_WAIT_EVENT_UNTIL(ELTRA_Check_event(ev,data,ISSUE_CARD_FROM_STK))

	/*
	 * @brief Set reader mode
	 */
	uint8_t ELTRA_ISO1443A_Request_Mode(void);
	bool ELTRA_GET_ISO1443A_UID();
	#define PROCESS_ELTRA_ISO1443A_REQUEST_MODE(); 													\
		ELTRA_Data_Send(ISO1443A_REQUEST_MODE, ELTRA_send_data, ELTRA_ISO1443A_Request_Mode());		\
		PROCESS_WAIT_EVENT_UNTIL(ELTRA_Check_event(ev,data,ISO1443A_REQUEST_MODE));					\
		free(ELTRA_send_data)


	uint8_t ELTRA_ISO1443A_Cascade_Level1(void);
	/*
	 * @brief read mifaire UID
	 * @param STATUS (ELTRA_card_error_t *) pointer to status of read
	 * @param UID (UID_t *) pointer to uid of card
	 */
	#define PROCESS_ELTRA_ISO1443A_CASCADE_LEVEL1(STATUS, UID); 									\
		ELTRA_Data_Send(ISO1443A_CASCADE_LEVEL1, ELTRA_send_data, ELTRA_ISO1443A_Cascade_Level1());	\
		PROCESS_WAIT_EVENT_UNTIL(ELTRA_Check_event(ev,data,ISO1443A_CASCADE_LEVEL1));				\
		ELTRA_GET_ISO1443A_UID(STATUS, UID);														\
		free(ELTRA_send_data)

	/*
	 * @brief
	 */
	bool ELTRA_GET_RFID_UID(ELTRA_card_error_t * Card_status, UID_t * UID);
	uint8_t ELTRA_RFID_transparent_command(char command);
	#define PROCESS_ELTRA_CONTINUOUS_READ(STATUS, UID); 														\
		ELTRA_Data_Send(RFID_TRANSPARENT_COMMAND, ELTRA_send_data, ELTRA_RFID_transparent_command('c'));		\
		PROCESS_WAIT_EVENT_UNTIL(ELTRA_Check_event(ev,data,RFID_TRANSPARENT_COMMAND));							\
		ELTRA_GET_RFID_UID(STATUS, UID);																		\
		free(ELTRA_send_data)
	/*
	 *@brief
	 */
	#define PROCESS_ELTRA_ABORT_CONTINUOUS_READ(); 																\
		ELTRA_Data_Send(RFID_TRANSPARENT_COMMAND, ELTRA_send_data, ELTRA_RFID_transparent_command('.'));		\
		PROCESS_WAIT_EVENT_UNTIL(ELTRA_Check_event(ev,data,RFID_TRANSPARENT_COMMAND));																																\
		free(ELTRA_send_data)


	uint8_t ELTRA_RFID_select(void);
	#define PROCESS_ELTRA_SELECT(ERROR); 														\
		ELTRA_Data_Send(RFID_TRANSPARENT_COMMAND, ELTRA_send_data, ELTRA_RFID_select());		\
		PROCESS_WAIT_EVENT_UNTIL(ELTRA_Check_event(ev,data,RFID_TRANSPARENT_COMMAND));			\
		ELTRA_GET_WRITE_STATUS(ERROR);															\
		free(ELTRA_send_data)

	uint8_t ELTRA_RFID_login(void);
	#define PROCESS_ELTRA_LOGIN(ERROR); 														\
		ELTRA_Data_Send(RFID_TRANSPARENT_COMMAND, ELTRA_send_data, ELTRA_RFID_login());			\
		PROCESS_WAIT_EVENT_UNTIL(ELTRA_Check_event(ev,data,RFID_TRANSPARENT_COMMAND));			\
		ELTRA_GET_WRITE_STATUS(ERROR);															\
		free(ELTRA_send_data)

	uint8_t ELTRA_RFID_Get_Info(void);
	bool ELTRA_GET_RFID_INFO_BLOCK(ELTRA_card_error_t * Card_status);
	#define PROCESS_ELTRA_READ_INFO_BLOCK(STATUS,ERROR); 										\
		ELTRA_Data_Send(RFID_TRANSPARENT_COMMAND, ELTRA_send_data, ELTRA_RFID_Get_Info());		\
		PROCESS_WAIT_EVENT_UNTIL(ELTRA_Check_event(ev,data,RFID_TRANSPARENT_COMMAND));			\
		STATUS = ELTRA_GET_RFID_INFO_BLOCK(ERROR);												\
		free(ELTRA_send_data)

	uint8_t ELTRA_RFID_Set_Info(void);
	bool ELTRA_GET_WRITE_STATUS(ELTRA_card_error_t * Card_status);
	#define PROCESS_ELTRA_WRITE_INFO_BLOCK(STATUS, ERROR); 										\
		ELTRA_Data_Send(RFID_TRANSPARENT_COMMAND, ELTRA_send_data, ELTRA_RFID_Set_Info());		\
		PROCESS_WAIT_EVENT_UNTIL(ELTRA_Check_event(ev,data,RFID_TRANSPARENT_COMMAND));			\
		STATUS = ELTRA_GET_WRITE_STATUS(ERROR);													\
		free(ELTRA_send_data)

#endif
