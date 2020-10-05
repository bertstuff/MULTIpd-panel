#ifndef EBM_MULTIPASS_IO_H_
#define EBM_MULTIPASS_IO_H_

	#include <stdbool.h>
	#include <stdint.h>

	#define EBM_RELAY1_PORT	1
	#define EBM_RELAY1_PIN 21
	#define EBM_RELAY2_PORT 1
	#define EBM_RELAY2_PIN 22
	#define EBM_RELAY3_PORT 1
	#define EBM_RELAY3_PIN 23
	#define EBM_RELAY4_PORT 1
	#define EBM_RELAY4_PIN 24

	#define EBM_INPUT1_PORT 1
	#define EBM_INPUT1_PIN 25
	#define EBM_INPUT2_PORT 1
	#define EBM_INPUT2_PIN 26
	#define EBM_INPUT3_PORT 1
	#define EBM_INPUT3_PIN 27

	#define EBM_RELAY_1 0
	#define EBM_RELAY_2 1
	#define EBM_RELAY_3 2
	#define EBM_RELAY_4 3

	#define EBM_INPUT_1 0
	#define EBM_INPUT_2 1
	#define EBM_INPUT_3 2
	/*
	 * @brief init io of EBM module
	 */
	void EBM_Multipass_io_init(void);

	/*
	 * @brief Set relay of EBM Multipass module on
	 * @param Relay relay to turn on  (EBM_RELAY_1..4)
	 */
	void EBM_Multipass_Set_output(uint8_t Relay);

	/*
	 * @brief Set relay of EBM Multipass module off
	 * @param Relay relay to turn off  (EBM_RELAY_1..4)
	 */
	void EBM_Multipass_Clear_output(uint8_t Relay);

	/*
	 * @brief Get state of input
	 * @param Input to read (EBM_INPUT_1..3)
	 */
	bool EBM_Multipass_Read_input(uint8_t Input);

	/*
	 * @brief get all input states
	 */
	uint8_t EBM_Multipass_Read_All_inputs(void);

#endif //EBM_MULTIPASS_IO_H_
