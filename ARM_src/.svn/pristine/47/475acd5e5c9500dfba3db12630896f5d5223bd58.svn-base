#ifndef _RTP_TYPES_H_
#define _RTP_TYPES_H_
	#include <stdint.h>
	#include <stdbool.h>
	#include <Core/protocol/Net/psock.h>
	#include <Core/Process/process.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>

	typedef enum{
		NO_ENCRYPTION = 0x0F,
		KEY_1 = 0x01,
	}RTP_Encryption_t;

	typedef enum{
		//Log commands
		CLEAR_LOG = 0x04, //Clears the internal log of the terminal (< v3.8)
		GET_LOG = 0x05,	//Retrieves the internal log of the terminal (< v3.8)
		GET_TRX_LOG = 0x14,	//Retrieve transaction log from the terminal
		CLEAR_LOG2 = 0x34, //Clears the internal log
		GET_LOG2 = 0x35, //Retrieves the internal log
		GET_LOG_CONFIG = 0x36, //Get log configuration
		SET_LOG_CONFIG = 0x37, //Set log configuration
		//User interface commands
		SET_MESSAGE = 0x06,  //Displays a message send to the terminal
		CLEAR_MESSAGE = 0x07,  //Clears the message on the display and shows the screensaver
		PLAY_BUZZER = 0x08,  //Plays the buzzer on the terminal
		SET_LED = 0x09,  //Turn on/off led on the terminal
		KEYPAD_SCAN = 0x0B, //Scans for a specific key
		KEY_EVENTS = 0x0D, //En/Disable key events.
		//Configuration commands
		SET_SETTINGS = 0x10, //Set configuration items in the terminal
		GET_SETTINGS = 0x11, //Get configuration items from the termina
		//Telemetry commands
		GET_EVA_DTS_DATA = 0x15, //Retrieve EVA-DTS data from the terminal
		//Timestamp commands
		SET_DATE_TIME = 0x20,  //Set Date and time.
		GET_DATE_TIME = 0x21,  //Retrieve Date and time.
		//Fingerprint reader
		CAPTURE_FINGER = 0x22,  //Capture finger template
		CANCEL_FINGER = 0x23,  //Cancel capture finger
		FINGER_TEMPLATE_EVENT = 0x24,  //Fingerprint event
		PVP_READ_FILE = 0x25, //PVP read telemetry file
		//Payment commands
		GET_TOKEN = 0xA0, //Get token for the specified application
		PERFORM_PAYMENT = 0xA1, //Perform a payment for the specified application
		RESET_PAYMENT = 0xA2, //Resets the payment engine in the terminal
		SHOW_BALANCE = 0xA3, //Shows / retrieves the balance for the specified application
		BUFFER_STATISTICS = 0xA4,  //Returns the buffer statistics for the specified application
		TRANSACTION_STATUS = 0xA7, //Returns the status of the last initiated transaction
		//Transparant commands
		INITIATE_SCAN = 0xB0, //Initiate scan for predefined cards
		PICC_EXCHANGE = 0xB1, //Exchange data with connected PICC
		CARD_INFO = 0xB6, //Returns card info after detecting a card
		RELEASE_CARD = 0xB3,  //Stops scanning for cards
		SET_MIFARE_KEYS = 0xB8, //Set the sector keys for the next call to read or write mifare blocks
		READ_MIFARE_BLOCKS = 0xB9, //Read a set of blocks from a Mifare card
		WRITE_MIFARE_BLOCKS = 0xBA, //Write a set of blocks to a Mifare card
		TRANSPARANT_EVENT = 0xAF,  //---
		SAM_POWER_ON = 0xB4,  //Power on a specified SAM
		SAM_POWER_OFF = 0xB5,  //Power off a specified SAM
		SAM_EXCHANGE = 0xB2,  //Exchange data with a powered SAM.
		//Test commands
		ECHO = 0x01, //	Echos the data send to the terminal
		//Firmware commands
		VERSION = 0x0A, //Get the version of the terminal
		RESTART = 0x55, //Restarts the terminal
		//EMV commands
		EMV_TRANSACTION = 0xE8, //EMV transaction
		EMV_CANCEL = 0xEA, //EMV cancel
		EMV_CLEARING = 0xEC,  //EMV clearing
		EMV_ICR_TRANSACTION = 0xED, //EMV ICR transaction
		EMV_ICR_ONLINE = 0xEF, //EMV ICR Online
		EMV_BATCH = 0x90, //EMV batch processing
	}RTP_CmdID_t;


	typedef enum{
		S_OK = 0, //Requested command executed successful
		S_NOK = 1, //Requested command failed
		S_ENC = 2, //Requested command not encrypted
		S_HDR = 3, //Unknown command requested
		S_PAR = 4, //Parameter error
		S_CRC = 5, //Incorrect CRC
		S_MSG = 6, //Expected command, message received
		S_LEN = 7, //Invalid length
		S_API = 8, //Application id command set not available
		S_SUB = 9, //No such sub command.
		S_INC = 10, //Incomplete result
		S_TIM = 11, //Timeout while executing command
		S_INVALID_STATE = 12, //Terminal is not in a state valid for this command
		S_FAIL = 13, //Command failed (special case of error). See command sections.
		S_INVALID_CARD = 14, //Invalid card for command.
		S_CANCELED = 15, //Scanning cancelled.
		S_INVALID_AMOUNT = 16, //Invalid amount in command
		S_SCAN_CARD = 17, //Scanning for card
		S_NO_SAM = 18, //No sam in terminal
		S_CARD_FOUND = 19, //Card in field
		S_NO_TAG = 20, //No tag within specified scan time
		S_TRANSACTION = 21, //Host transaction started
		S_PAY_OK = 22, //Payment complete
		S_PAY_BUF = 23, //Payment complete but buffered
		S_PAY_FAIL = 24, //Payment failed
		S_BAL_FAIL = 25, //Balance failed
		S_BAL_OK = 26, //Balance complate
		S_CARD_AUTH_FAIL = 27, //Failed to authenticate with card.
		S_TIMEOUT = 100, //connection timeout
		S_CONN_ERROR = 101, //connection error
	}RTP_Status_t;

	typedef struct{
		uint16_t AID;	//Application ID, possible values are defined in the individual command sections.
		uint8_t CmdID;	//Command identifier, identifier of command. Values are defined in the individual command sections.
		uint8_t SeqNr;	//Sequence number, sequence number of command sequence. Each Command/Message pair have a matching sequence number.
		uint8_t Opt;	//Option flags, bitwise field for frame options.
		uint8_t Res;	//Reserved for future use
		uint16_t Len;	//Length of the payload (data). This length is adjusted if the frame is encrypted to contain the length of the encrypted frame.
		uint32_t CRC32;	//CRC32 of the frame.
	}RTP_frame_t;

	typedef struct{
		//RTP frame
		RTP_frame_t frame;
		RTP_frame_t received_frame;

		//Payload data of the message
		uint8_t * Data;
		uint8_t * received_Data;

		//send status
		RTP_Status_t status;

		//process status
		struct pt p;
		struct psock ps;
	}RTP_packet_t;

	extern bool RTP_busy;
#endif //_RTP_TYPES_H_
