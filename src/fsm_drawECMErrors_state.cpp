#include "ch32v003fun.h"
#include "include/cube_defs.h"

#include "include/FSM.h"

extern tim2Encoder enc;
extern sh1106 OLEDScreen;
extern ALDLParser myALDLParser;
extern bool fsm_enter_state_flag;
extern void ( *fsm_state )( void );

#ifdef ECM_DEBUG
	extern A172ALDL ALDLData;
#endif

void fsm_drawECMErrors_state()
{
	// Declare local/static variable here.
	static uint8_t errPointer;
	static uint8_t errCount;
	static char** errTexts;
	

	if ( fsm_enter_state_flag )
	{
		// Run once when enter this state.
		makeScreen(87, 0, errors_bitmap, 32, 8);
		
		// Here we parse and show errors
		myALDLParser.parse();
		errCount  = myALDLParser.getErrCount();
		errTexts  = myALDLParser.getErrTexts();
		errPointer = 0;

		// Draw scrolling circle
		if(errCount > LINES_MAX)
			OLEDScreen.drawCircle(124, 6, 1, 1);

		if(errCount == 0)
		{
			OLEDScreen.drawstr_sz(8, lineNumbers[3], (char*)"No errors!", 1, fontsize_10x16);
		}
		else
		{
			uint8_t currStr = 0;
			for(uint8_t i = errPointer; i < (errPointer + LINES_MAX); i++)
			{
				OLEDScreen.drawstr(3, lineNumbers[currStr], errTexts[i], 1);
				currStr++;
				if (currStr >= errCount)
				{
					break;
				}

			}
		}

		OLEDScreen.refresh();
	}
	
	// Run repeatly for update.
	
	// We run this only when there is more than LINES_MAX errors and 
	// there is any input from encoder.
	int8_t delta = enc.getDelta();
	if( (delta != 0) && (errCount > LINES_MAX) )
	{
		if( ((-delta) > errPointer) || ((delta + errPointer) > (errCount - LINES_MAX)) )
			delta = 0;

		// Clear screen
		CLS();

		// Draw scrolling circle
		OLEDScreen.drawCircle(124, map(errPointer, 0, (errCount - LINES_MAX), 6, 58), 1, 1);

		errPointer += delta;
		uint8_t currStr = 0;
		for(uint8_t i = errPointer; i < (errPointer + LINES_MAX); i++)
		{
			OLEDScreen.drawstr(3, lineNumbers[currStr], errTexts[i], 1);
			currStr++;
			if (currStr >= errCount)
			{
				break;
			}
			
		}
	
		OLEDScreen.refresh();
	}


	if ( btnClick() )
	{
		fsm_state = &fsm_drawECMParametersTemp_state;
		fsm_enter_state_flag = true;
		return;
	}

	if ( btnHeld() )
	{
		fsm_state = &fsm_resetECMErrors_state;
		fsm_enter_state_flag = true;
		return;
	}
	fsm_enter_state_flag = false; // Reset flag
}