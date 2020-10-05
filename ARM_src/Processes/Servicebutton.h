#ifndef SERVICEBUTTON
#define SERVICEBUTTON
#include <Core/Thread/pt.h>
#include <Core/Process/process.h>
  /* Global prototypes */
extern struct pt pin_pt;

/*
 * @brief standaard handler for checking service button(sw1 button1) and service pass
 * @param exit when exit returns true the current screen must be closed
 * @param ev event
 * @param data event data
 */
PT_THREAD(Servicebutton_handler(bool * exit,process_event_t ev, process_data_t data));

#endif
 /**
 * 	@}
 * @}
 **/
