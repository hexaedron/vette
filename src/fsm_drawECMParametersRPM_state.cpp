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

void fsm_drawECMParametersRPM_state()
{
	// Declare local/static variable here.

	if ( fsm_enter_state_flag )
	{
		// Run once when enter this state.
		makeScreen(87, 0, params_bitmap, 32, 8);
	}
	
	// Run repeatly for update.
	
	#ifdef ECM_DEBUG
		ALDLData.NTRPMX  = 87;
		ALDLData.ISESDD  = 95;
		ALDLData.NOCKRTD = 6;
		ALDLData.NTPSLD  = 25;
	#endif

	if ( btnClick() )
	{
		fsm_state = &fsm_drawECMParametersBLM_state;
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

	
	// Here we print everything RPM related
	uint8_t printPos;

	OLEDScreen.drawstr(21, lineNumbers[0] + 1, (char*)"RPM", 1);
	printPos = (128 - (strlen(myALDLParser.getRPM()) + 3) * 10) / 2 - 22;
	OLEDScreen.drawstr_sz(printPos, lineNumbers[2] - 1, myALDLParser.getRPM(), 1, fontsize_10x16);

	printPos = (128 - (strlen(myALDLParser.getKnockrtdDeg()) + 3) * 10) / 2 + 36;
	OLEDScreen.drawstr(64, lineNumbers[0] + 1, (char*)"Knock Ret%", 1);
	OLEDScreen.drawstr_sz(printPos, lineNumbers[2] - 1, myALDLParser.getKnockrtdDeg(), 1, fontsize_10x16);

	OLEDScreen.drawstr(4, lineNumbers[3] + 2, (char*)"Idle RPM", 1);
	printPos = (128 - (strlen(myALDLParser.getDesiredIdleRPM()) + 3) * 10) / 2 - 22;
	OLEDScreen.drawstr_sz(printPos, lineNumbers[5], myALDLParser.getDesiredIdleRPM(), 1, fontsize_10x16);

	printPos = (128 - (strlen(myALDLParser.getTPSPct()) + 3) * 10) / 2 + 36;
	OLEDScreen.drawstr(72, lineNumbers[3] + 2, (char*)"TPS%", 1);
	OLEDScreen.drawstr_sz(printPos, lineNumbers[5], myALDLParser.getTPSPct(), 1, fontsize_10x16);

	OLEDScreen.refresh();

}