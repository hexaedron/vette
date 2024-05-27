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

void fsm_drawABSParameters_state()
{
	// Declare local/static variable here.

	if ( fsm_enter_state_flag )
	{
		// Run once when enter this state.
		makeScreen(94, 0, abs_bitmap, 16, 8);
		OLEDScreen.drawImage(24, 16, vette_top_bitmap, 64, 32, 0);
	}
	
	// Run repeatly for update.

	#ifdef ECM_DEBUG
		ABSData.LFWheelSpeed = 47;
		ABSData.LRWheelSpeed = 47;
		ABSData.RFWheelSpeed = 47;
		ABSData.RRWheelSpeed = 47;
	#endif

	if ( btnClick() )
	{
		fsm_state = &fsm_drawABSErrors_state;
		fsm_enter_state_flag = true;
		return;
	}
	fsm_enter_state_flag = false; // Reset flag

	getABSData();
	#ifndef ECM_DEBUG
		if(!fixAndCheckABSData(&ABSData))
		{
			fsm_enter_state_flag = false;
			return;
		}
	#endif

	OLEDScreen.drawstr(80, lineNumbers[0] + 3, getPaddedSpeed(ABSData.LFWheelSpeed), 1);
	OLEDScreen.drawstr(30, lineNumbers[0] + 3, getPaddedSpeed(ABSData.LRWheelSpeed), 1);
	OLEDScreen.drawstr(30, lineNumbers[5] - 3, getPaddedSpeed(ABSData.RRWheelSpeed), 1);
	OLEDScreen.drawstr(80, lineNumbers[5] - 3, getPaddedSpeed(ABSData.RFWheelSpeed), 1);

	OLEDScreen.refresh();	

}