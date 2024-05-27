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

void fsm_drawFanStatus_state()
{
	// Declare local/static variable here.

	if ( fsm_enter_state_flag )
	{
		// Run once when enter this state.
		makeScreen(94, 0, fan_bitmap, 16, 8);
		OLEDScreen.drawImage(12, lineNumbers[0] - 6, fan_propeller_bitmap, 24, 24, 0);
		OLEDScreen.drawImage(76, lineNumbers[0] - 6, fan_propeller_bitmap, 24, 24, 0);
		OLEDScreen.drawchar(42,  lineNumbers[1] - 2, '1', 1);
		OLEDScreen.drawchar(108, lineNumbers[1] - 2, '2', 1);
	}
	
	// Run repeatly for update.
	#ifdef ECM_DEBUG
		ALDLData.FANMW  = 0b01111111;
	#endif

	if ( btnClick() )
	{
		fsm_state = &fsm_drawECMErrors_state;
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

	if(bitRead(ALDLData.FANMW, 2))
	{
		OLEDScreen.drawstr(4, lineNumbers[3], (char*)"Request", 1);
	}
	else
	{
		OLEDScreen.drawstr(4, lineNumbers[3], (char*)"       ", 1);
	}

	if(bitRead(ALDLData.FANMW, 0))
	{
		OLEDScreen.drawstr(4, lineNumbers[4], (char*)"Enabl    ", 1);
	}
	else
	{
		OLEDScreen.drawstr(4, lineNumbers[4], (char*)"         ", 1);
	}

	if(bitRead(ALDLData.FANMW, 5))
	{
		OLEDScreen.drawstr(4, lineNumbers[4], (char*)"Enable (oil)", 1);
	}

	if(bitRead(ALDLData.FANMW, 4))
	{
		OLEDScreen.drawstr(81, lineNumbers[3], (char*)"Request", 1);
	}
	else
	{
		OLEDScreen.drawstr(81, lineNumbers[3], (char*)"       ", 1);
	}

	if(bitRead(ALDLData.FANMW, 3))
	{
		OLEDScreen.drawstr(81, lineNumbers[4], (char*)"Enable", 1);
	}
	else
	{
		OLEDScreen.drawstr(81, lineNumbers[4], (char*)"      ", 1);
	}

	if (bitRead(ALDLData.FANMW, 6))
	{
		OLEDScreen.drawstr_sz(55, lineNumbers[1], (char*)"Hi", 1, fontsize_10x16);
	}
	else
	{
		OLEDScreen.drawstr_sz(55, lineNumbers[1], (char*)"Lo", 1, fontsize_10x16);
	}

	if(bitRead(ALDLData.FANMW, 1))
	{
		OLEDScreen.fillRect(25, lineNumbers[5] - 8, 66, 8, 1);
		OLEDScreen.drawstr(26, lineNumbers[5], (char*)"SPI failed!", 0);
	}
	else
	{
		OLEDScreen.fillRect(25, lineNumbers[5] - 8, 66, 8, 0);
	}
	
	
	OLEDScreen.refresh();
}