#include <Board/Def_config.h>
#if (RESERVATION_MODULE == 1)
	#include <stdbool.h>
	#include <stdio.h>
	#include <stdint.h>
	#include <time.h>
	#include <Core/Process/process.h>

	extern struct pt resv_pt;
	extern struct tm g_Start_time;
	extern struct tm g_End_time;
	extern int32_t g_Cur_ResvNr;

	typedef struct{
		bool Resv_ok;
		struct tm Start_time;
		struct tm End_time;
		struct tm Cur_time;
		double lenght;
		double time_to_end;
		double time_to_start;
	}Reservation_time_t;

	PT_THREAD(get_reservation_time(Reservation_time_t * resv_time, process_event_t ev, process_data_t data));
	PT_THREAD(show_reservation_message(Reservation_time_t resv_time ,process_event_t ev, process_data_t data));

#endif
