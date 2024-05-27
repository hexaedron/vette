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

void fsm_drawECMParametersBLM_state()
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
		ALDLData.LBLM    = 131;
		ALDLData.RBLM    = 124;
		ALDLData.LINT    = 135;
		ALDLData.RINT    = 120;
		ALDLData.BLMCELL = 7;
	#endif

	if ( btnClick() )
	{
		fsm_state = &fsm_drawECMParametersPressure_state;
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

	
	// Here we print everything BLM related
	// https://turbobuick.com/threads/better-understand-your-blm-and-int.278346/
	// https://www.motortrend.com/how-to/1605-diagnosing-a-failed-or-clogged-fuel-injector-on-1982-95-corvettes-with-obd-i/

	uint8_t printPos;

	OLEDScreen.drawstr(8, lineNumbers[0] + 1, (char*)"L Int%", 1);
	printPos = (128 - (strlen(myALDLParser.getLINTPct()) + 3) * 10) / 2 - 22;
	OLEDScreen.drawstr_sz(printPos, lineNumbers[2] - 1, myALDLParser.getLINTPct(), 1, fontsize_10x16);

	printPos = (128 - (strlen(myALDLParser.getRINTPct()) + 3) * 10) / 2 + 36;
	OLEDScreen.drawstr(64, lineNumbers[0] + 1, (char*)"R Int%", 1);
	OLEDScreen.drawstr_sz(printPos, lineNumbers[2] - 1, myALDLParser.getRINTPct(), 1, fontsize_10x16);

	if(myALDLParser.getBLMCell()[1] == '\0') // 1 digit (instead of strlen())
	{
		printPos = 55;
	}
	else
	{
		printPos = 51;
	}

	OLEDScreen.fillRect(50, 30, 14, 11, 1);
	OLEDScreen.drawstr(printPos, 39, myALDLParser.getBLMCell(), 0);
	

	OLEDScreen.drawstr(8, lineNumbers[3] + 2, (char*)"L BLM%", 1);
	printPos = (128 - (strlen(myALDLParser.getLBLMPct()) + 3) * 10) / 2 - 22;
	OLEDScreen.drawstr_sz(printPos, lineNumbers[5], myALDLParser.getLBLMPct(), 1, fontsize_10x16);

	printPos = (128 - (strlen(myALDLParser.getRBLMPct()) + 3) * 10) / 2 + 43;
	OLEDScreen.drawstr(72, lineNumbers[3] + 2, (char*)"R BLMT%", 1);
	OLEDScreen.drawstr_sz(printPos, lineNumbers[5], myALDLParser.getRBLMPct(), 1, fontsize_10x16);

	OLEDScreen.refresh();

}
