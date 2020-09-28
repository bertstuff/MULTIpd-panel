volatile char *Build_Revision      = "7";
volatile char *Build_Date          = "2019/05/24 10:33:03";

#ifndef DEBUGGING
	#if 1
		#warning Source is modified
		volatile char *Version_state = "C";
	#else
		volatile char *Version_state = "R";
	#endif
#else
	volatile char *Version_state = "D";
	#warning Debugging enabled
	#if 1
		#warning Source is modified
	#endif
#endif