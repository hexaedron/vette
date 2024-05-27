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

void fsm_drawECMParametersTemp_state()
{
	// Declare local/static variable here.

	if ( fsm_enter_state_flag )
	{
		// Run once when enter this state.
		makeScreen(87, 0, params_bitmap, 32, 8);
	}
	
	// Run repeatly for update.
	
	#ifdef ECM_DEBUG
		ALDLData.NTRPMX = 87;
		ALDLData.ADOILTMP = 85;
		ALDLData.COOLDEGA = 213;		
		ALDLData.ADMAT = 36;
	#endif

	if ( btnClick()  )
	{
		fsm_state = &fsm_drawECMParametersRPM_state;
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

	
	// Here we print everything temperature related
	uint8_t printPos;

	OLEDScreen.drawstr(45, lineNumbers[0] - 1, (char*)"Temp C", 1);
	printPos = (138 - (strlen(myALDLParser.getCoolC()) + 3) * 10) / 2;
	OLEDScreen.drawstr_sz(printPos, lineNumbers[2] - 1, myALDLParser.getCoolC(), 1, fontsize_10x16);

	OLEDScreen.drawstr(4, lineNumbers[3], (char*)"Oil Temp", 1);
	printPos = (128 - (strlen(myALDLParser.getOilTempC()) + 3) * 10) / 2 - 22;
	OLEDScreen.drawstr_sz(printPos, lineNumbers[5], myALDLParser.getOilTempC(), 1, fontsize_10x16);

	printPos = (128 - (strlen(myALDLParser.getOilTempC()) + 3) * 10) / 2 + 43;
	OLEDScreen.drawstr(72, lineNumbers[3], (char*)"MAT Temp", 1);
	OLEDScreen.drawstr_sz(printPos, lineNumbers[5], myALDLParser.getMatTempC(), 1, fontsize_10x16);

	OLEDScreen.refresh();
}
