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

void fsm_resetECMErrors_state()
{
	// Declare local/static variable here.
	static bool clearFlag = false;
	static bool refreshFlag = false;

	if ( fsm_enter_state_flag )
	{
		// Run once when enter this state.
		OLEDScreen.setbuf(0);
		OLEDScreen.drawFrame(1);
		OLEDScreen.drawstr(32, lineNumbers[1], (char*)"Clear errors?", 1);
		OLEDScreen.drawstr_sz(16, lineNumbers[4], (char*)"Yes", 1, fontsize_15x24);
		OLEDScreen.drawstr_sz(80, lineNumbers[4], (char*)"No", 1, fontsize_15x24);
		OLEDScreen.xorrect(79, lineNumbers[2] - 4, 35, 23);
	}
	
	// Run repeatly for update.

	if(enc.getDelta() != 0)
	{
		clearFlag = !clearFlag;
		refreshFlag = true;
	}

	if(refreshFlag)
	{
		OLEDScreen.xorrect(15, lineNumbers[2] - 4, 53, 23);
		OLEDScreen.xorrect(79, lineNumbers[2] - 4, 35, 23);
		refreshFlag = false;
	}

	OLEDScreen.refresh();


	if ( btnClick()  )
	{
		if(clearFlag)
		{
			flushADLDErrors();
			getADLDData();

			#ifdef ECM_DEBUG
				ALDLData.MALFFLG1 = 0;
				ALDLData.MALFFLG1 = 0;
				ALDLData.MALFFLG3 = 0;
				ALDLData.MALFFLG4 = 0;
				ALDLData.MALFFLG5 = 0;
			#endif
		}
		clearFlag = false;
		refreshFlag = false;

		fsm_state = &fsm_drawECMErrors_state;
		fsm_enter_state_flag = true;
		return;
	}
	fsm_enter_state_flag = false; // Reset flag
}
