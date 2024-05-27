#include "ch32v003fun.h"
#include "include/cube_defs.h"

#include "include/FSM.h"

extern tim2Encoder enc;
extern sh1106 OLEDScreen;
extern bool fsm_enter_state_flag;
extern void ( *fsm_state )( void );

#ifdef ECM_DEBUG
	extern A172ALDL ALDLData;
#endif

void fsm_selectMode_state()
{
	// Declare local/static variable here.
	static bool modeFlag = false;
	static bool refreshFlag = false;

	if ( fsm_enter_state_flag )
	{
		// Run once when enter this state.
		OLEDScreen.setbuf(0);
		OLEDScreen.drawFrame(1);
		OLEDScreen.drawstr(35, lineNumbers[1], (char*)"Select mode:", 1);
		OLEDScreen.drawstr_sz(8, lineNumbers[4], (char*)"ABS", 1, fontsize_15x24);
		OLEDScreen.drawstr_sz(72, lineNumbers[4], (char*)"ECM", 1, fontsize_15x24);
		OLEDScreen.xorrect(71, lineNumbers[2] - 4, 53, 23);
	}
	
	// Run repeatly for update.

	if(enc.getDelta() != 0)
	{
		modeFlag = !modeFlag;
		refreshFlag = true;
	}

	if(refreshFlag)
	{
		OLEDScreen.xorrect(7, lineNumbers[2] - 4, 53, 23);
		OLEDScreen.xorrect(71, lineNumbers[2] - 4, 53, 23);
		refreshFlag = false;
	}

	OLEDScreen.refresh();


	if ( btnClick()  )
	{
		if(!modeFlag)
		{
			fsm_state = &fsm_connectECM_state;
			fsm_enter_state_flag = true;
			return;
		}
		else
		{
			fsm_state = &fsm_instructionsForABS_state;
			fsm_enter_state_flag = true;
			return;
		}
	}
	fsm_enter_state_flag = false; // Reset flag
}