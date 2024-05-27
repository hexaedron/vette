#include "ch32v003fun.h"
#include "include/cube_defs.h"

#include "include/FSM.h"

extern sh1106 OLEDScreen;
extern ABSALDL ABSData;
extern bool fsm_enter_state_flag;
extern void ( *fsm_state )( void );

#ifdef ECM_DEBUG
	extern A172ALDL ALDLData;
#endif

void fsm_drawABSErrors_state()
{
	// Declare local/static variable here.

	ABSParser myABSParser;

	if ( fsm_enter_state_flag )
	{
		// Run once when enter this state.
		makeScreen(84, 0, abs_err_bitmap, 32, 8);
	}

	if ( btnClick() )
	{
		fsm_state = &fsm_drawABSParameters_state;
		fsm_enter_state_flag = true;
		return;
	}
	fsm_enter_state_flag = false; // Reset flag

	getABSData();
	#ifndef ECM_DEBUG
		if(!myABSParser.fixAndCheckABSData(&ABSData))
		{
			fsm_enter_state_flag = false;
			return;
		}
	#endif


	#ifdef ECM_DEBUG
		ABSData.fc1.faultCodeNum = 66;
		ABSData.fc2.faultCodeNum = 41;
		ABSData.fc3.faultCodeNum = 74;
	#endif

	if( (ABSData.fc1.faultCodeNum == 0xFF) && (ABSData.fc2.faultCodeNum == 0xFF) && (ABSData.fc3.faultCodeNum == 0xFF))
	{
		OLEDScreen.drawstr_sz(8, lineNumbers[3], (char*)"No errors!", 1, fontsize_10x16);
	}
	else
	{
		OLEDScreen.drawstr(4, lineNumbers[1] - 4, (char*)myABSParser.getABSMessage(ABSData.fc1.faultCodeNum), 1);
		OLEDScreen.drawstr(4, lineNumbers[3] - 4, (char*)myABSParser.getABSMessage(ABSData.fc2.faultCodeNum), 1);
		OLEDScreen.drawstr(4, lineNumbers[5] - 4, (char*)myABSParser.getABSMessage(ABSData.fc3.faultCodeNum), 1);
	}
	
	OLEDScreen.refresh();

	// Run repeatly for update.
}