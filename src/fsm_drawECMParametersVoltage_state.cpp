#include "ch32v003fun.h"
#include "include/cube_defs.h"

#include "include/FSM.h"

extern sh1106 OLEDScreen;
extern ALDLParser myALDLParser;
extern bool fsm_enter_state_flag;
extern void ( *fsm_state )( void );

#ifdef ECM_DEBUG
	extern A172ALDL ALDLData;
#endif

void fsm_drawECMParametersVoltage_state()
{
	// Declare local/static variable here.

	if ( fsm_enter_state_flag )
	{
		// Run once when enter this state.
		makeScreen(87, 0, params_bitmap, 32, 8);
	}
	
	// Run repeatly for update.
	
	#ifdef ECM_DEBUG
		ALDLData.ADBAT = 131;
	#endif

	if ( btnClick() )
	{
		fsm_state = &fsm_drawFanStatus_state;
		fsm_enter_state_flag = true;
		return;
	}
	fsm_enter_state_flag = false; // Reset flag

	getADLDData();
	#ifndef ECM_DEBUG
		if(!myALDLParser.validateChecksum())
		{
			fsm_enter_state_flag = false;
			return;
		}
	#endif

	CLS();

	
	// Here we print voltage
	uint8_t printPos;

	OLEDScreen.drawstr(35, lineNumbers[1], (char*)"Voltage:", 1);
	printPos = (138 - (strlen(myALDLParser.getVoltage()) + 1) * 15) / 2;
	OLEDScreen.drawstr_sz(printPos, lineNumbers[4], myALDLParser.getVoltage(), 1, fontsize_15x24);

	OLEDScreen.refresh();

}