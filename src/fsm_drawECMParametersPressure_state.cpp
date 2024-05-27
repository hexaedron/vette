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

void fsm_drawECMParametersPressure_state()
{
	// Declare local/static variable here.

	if ( fsm_enter_state_flag )
	{
		// Run once when enter this state.
		makeScreen(87, 0, params_bitmap, 32, 8);
	}
	
	// Run repeatly for update.
	
	#ifdef ECM_DEBUG
		ALDLData.ADBAT    = 131;
		ALDLData.ADMAP    = 131;
		ALDLData.EGRDC    = 120;
		ALDLData.NVADBARO = 170;
	#endif

	if ( btnClick() )
	{
		fsm_state = &fsm_drawECMParametersVoltage_state;
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
	
	// Here we print everything engine related
	uint8_t printPos;

	OLEDScreen.drawstr(8, lineNumbers[0] + 1, (char*)"EGR duty%", 1);
	printPos = (128 - (strlen(myALDLParser.getEGRDutyPct()) + 3) * 10) / 2 - 22;
	OLEDScreen.drawstr_sz(printPos, lineNumbers[2] - 1, myALDLParser.getEGRDutyPct(), 1, fontsize_10x16);

	printPos = (128 - (strlen(myALDLParser.getTPSPct()) + 3) * 10) / 2 + 36;
	OLEDScreen.drawstr(72, lineNumbers[0] + 1, (char*)"TPS%", 1);
	OLEDScreen.drawstr_sz(printPos, lineNumbers[2] - 1, myALDLParser.getTPSPct(), 1, fontsize_10x16);

	OLEDScreen.drawstr(8, lineNumbers[3] + 2, (char*)"MAP kPa", 1);
	printPos = (128 - (strlen(myALDLParser.getMAPkpa()) + 3) * 10) / 2 - 22;
	OLEDScreen.drawstr_sz(printPos, lineNumbers[5], myALDLParser.getMAPkpa(), 1, fontsize_10x16);

	printPos = (128 - (strlen(myALDLParser.getBarometerkpa()) + 3) * 10) / 2 + 43;
	OLEDScreen.drawstr(72, lineNumbers[3] + 2, (char*)"BARO kPa", 1);
	OLEDScreen.drawstr_sz(printPos, lineNumbers[5], myALDLParser.getBarometerkpa(), 1, fontsize_10x16);

	OLEDScreen.refresh();
}
