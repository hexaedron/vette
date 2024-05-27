#include "ch32v003fun.h"
#include "include/cube_defs.h"

#include "include/FSM.h"

extern sh1106 OLEDScreen;
extern A172ALDL ALDLData;
extern ALDLParser myALDLParser;
extern bool fsm_enter_state_flag;
extern void ( *fsm_state )( void );

#ifdef ECM_DEBUG
	extern A172ALDL ALDLData;
#endif

void fsm_connectECM_state()
{
	// Declare local/static variable here.

	if ( fsm_enter_state_flag )
	{
		// Run once when enter this state.
		makeScreen(87, 0, 0, 32, 8);
		OLEDScreen.drawstr(16, lineNumbers[2], (char*)"Initializing PCM", 1);
		OLEDScreen.drawstr(26, lineNumbers[3], (char*)"Connection...", 1);
		OLEDScreen.refresh();

		// Populate ALDLData
		// We wait for ECM response. If there is no response, we have nothing to do here.
		getADLDData();
		#ifndef ECM_DEBUG
			while (!myALDLParser.validateChecksum())
			{
				getADLDData();
			}
		#endif
		

		#ifdef ECM_DEBUG
			ALDLData.MALFFLG1 = 0xFF;
			ALDLData.MALFFLG1 = 0xFF;
			ALDLData.MALFFLG3 = 0xFF;
			ALDLData.MALFFLG4 = 0b11010111;
			ALDLData.MALFFLG5 = 0xFF;
		#endif
	}

	if ( true )
	{
		fsm_state = &fsm_drawECMErrors_state;
		fsm_enter_state_flag = true;
		return;
	}

	fsm_enter_state_flag = false; // Reset flag
}
