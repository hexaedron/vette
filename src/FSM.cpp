#include "ch32v003fun.h"
#include "include/cube_defs.h"

#include "include/FSM.h"
#include "include/tim2Encoder.h"
#include "include/simpleTimer.h"
#include "sh1106.h"
#include "include/UART.h"

#include "include/bitmaps.h"
#include "include/line_nums.h"

#include "include/ALDL_cmd.h"
#include "include/A172ALDL.h"
#include "include/ABSALDL.h"

#include <string.h>    // strlen
#include <stdlib.h>    // itoa
#include <stdio.h>	   // printf

#define ECM_DEBUG

// from system.cpp
bool btnPressed(uint32_t);

tim2Encoder enc(AFIO_PCFR1_TIM2_REMAP_NOREMAP);
sh1106 OLEDScreen;
UART ALDL_UART;
A172ALDL ALDLData = {0};
ALDLParser ALDLParser;

// https://menginventor.github.io/FSM_coder/#


// This section gennerated by FSM Coder
void ( *fsm_state )( void ); // use function pointer as state variable;
bool fsm_enter_state_flag = true;

void fsm_init()
{
	fsm_state = &fsm_init_state;
}

void fsm_update()
{
	( *fsm_state )(); // call FSM state
}

// ****************************************************************************************

void fsm_init_state()
{
	// Declare local/static variable here.

	if ( fsm_enter_state_flag )
	{
		// Run once when enter this state.
        ALDL_UART.beginHD(8192);

        OLEDScreen.init();
        makeStartScreen();
        OLEDScreen.refresh();
	}
	// Run repeatly for update.


	if ( btnPressed(PC6) )
	{
		fsm_state = &fsm_connectECM_state;
		fsm_enter_state_flag = true;
		return;
	}

	fsm_enter_state_flag = false; // Reset flag
}

void fsm_connectECM_state()
{
	// Declare local/static variable here.

	if ( fsm_enter_state_flag )
	{
		// Run once when enter this state.
		makeScreen(87, 0, 0, 32, 8);
		OLEDScreen.drawstr(16, lineNumbers[2], (char*)"Initializing PCM", 1);
		OLEDScreen.drawstr(26, lineNumbers[3], (char*)"Connection...", 1);
		OLEDScreen.refresh();

		// We wait for ECM response. If there is no response, we have nothing to do here.
		waitForECMSync();

		// Populate ALDLData
		getADLDData();
	}
	// Run repeatly for update.


	if ( true )
	{
		fsm_state = &fsm_drawECMErrors_state;
		fsm_enter_state_flag = true;
		return;
	}

	fsm_enter_state_flag = false; // Reset flag
}

// ****************************************************************************************

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

		#ifdef ECM_DEBUG
			ALDLData.MALFFLG1 = 0xFF;
			ALDLData.MALFFLG1 = 0xFF;
			ALDLData.MALFFLG3 = 0xFF;
			ALDLData.MALFFLG4 = 0b11010111;
			ALDLData.MALFFLG5 = 0xFF;
		#endif


		ALDLParser.attach(&ALDLData);
		ALDLParser.parse();
		errCount  = ALDLParser.getErrCount();
		errTexts  = ALDLParser.getErrTexts();
		errPointer = 0;

		// Draw scrolling circle
		if(errCount > LINES_MAX)
			OLEDScreen.drawCircle(124, 6, 1, 1);

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
	
	// Run repeatly for update.
	
	// We run this only when there is more than LINES_MAX errors and 
	// there is any input from encoder.
	int8_t delta = enc.getDelta();
	if( (delta != 0) && (errCount > LINES_MAX) )
	{
		if( ((-delta) > errPointer) || ((delta + errPointer) > (errCount - LINES_MAX)) )
			delta = 0;

		// Clear screen
		OLEDScreen.fillRect(2, 4, 125, 58, 0);
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


	if ( btnPressed(PC6) )
	{
		fsm_state = &fsm_drawECMParameters1_state;
		fsm_enter_state_flag = true;
		return;
	}
	fsm_enter_state_flag = false; // Reset flag
}

// ****************************************************************************************

void fsm_drawECMParameters1_state()
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

	getADLDData();

	uint8_t printPos;

	OLEDScreen.drawstr(51, lineNumbers[0] - 1, (char*)"Temp:", 1);
	printPos = (138 - (strlen(ALDLParser.getCoolC()) + 3) * 10) / 2;
	OLEDScreen.drawstr_sz(printPos, lineNumbers[2] - 1, ALDLParser.getCoolC(), 1, fontsize_10x16);

	OLEDScreen.drawstr(4, lineNumbers[3], (char*)"Oil Temp:", 1);
	printPos = (128 - (strlen(ALDLParser.getOilTempC()) + 3) * 10) / 2 - 22;
	OLEDScreen.drawstr_sz(printPos, lineNumbers[5], ALDLParser.getOilTempC(), 1, fontsize_10x16);

	printPos = (128 - (strlen(ALDLParser.getOilTempC()) + 3) * 10) / 2 + 43;
	OLEDScreen.drawstr(72, lineNumbers[3], (char*)"MAT Temp:", 1);
	OLEDScreen.drawstr_sz(printPos, lineNumbers[5], ALDLParser.getMatTempC(), 1, fontsize_10x16);

	OLEDScreen.refresh();


	if ( btnPressed(PC6)  )
	{
		fsm_state = &fsm_drawECMParameters2_state;
		fsm_enter_state_flag = true;
		return;
	}
	fsm_enter_state_flag = false; // Reset flag
}

// ****************************************************************************************

void fsm_drawABSParameters_state()
{
	// Declare local/static variable here.

	if ( fsm_enter_state_flag )
	{
		// Run once when enter this state.
		makeScreen(94, 0, abs_bitmap, 16, 8);
		OLEDScreen.drawImage(24, 16, vette_top_bitmap, 64, 32, 0);
		OLEDScreen.refresh();
	}
	// Run repeatly for update.


	if ( btnPressed(PC6) )
	{
		fsm_state = &fsm_drawECMErrors_state;
		fsm_enter_state_flag = true;
		return;
	}
	fsm_enter_state_flag = false; // Reset flag
}

// ****************************************************************************************

void fsm_drawECMParameters2_state()
{
	// Declare local/static variable here.

	if ( fsm_enter_state_flag )
	{
		// Run once when enter this state.
	}
	// Run repeatly for update.


	if ( btnPressed(PC6) )
	{
		fsm_state = &fsm_drawFanStatus_state;
		fsm_enter_state_flag = true;
		return;
	}
	fsm_enter_state_flag = false; // Reset flag
}

void fsm_drawABSErrors_state()
{
	// Declare local/static variable here.

	if ( fsm_enter_state_flag )
	{
		// Run once when enter this state.
		makeScreen(84, 0, abs_err_bitmap, 32, 8);
		OLEDScreen.refresh();
	}
	// Run repeatly for update.


	if ( btnPressed(PC6) )
	{
		fsm_state = &fsm_drawABSParameters_state;
		fsm_enter_state_flag = true;
		return;
	}
	fsm_enter_state_flag = false; // Reset flag
}

// ****************************************************************************************

void fsm_drawFanStatus_state()
{
	// Declare local/static variable here.

	if ( fsm_enter_state_flag )
	{
		// Run once when enter this state.
		makeScreen(94, 0, fan_bitmap, 16, 8);
		OLEDScreen.refresh();
	}
	// Run repeatly for update.


	if ( btnPressed(PC6) )
	{
		fsm_state = &fsm_drawABSErrors_state;
		fsm_enter_state_flag = true;
		return;
	}
	fsm_enter_state_flag = false; // Reset flag
}

// ****************************************************************************************

void waitForECMSync(void)
{
	// Init ECM conneсtion. ECM should respond with 4 bytes
	// We have nothing to do if there is no response, so we try
	// again and again
	#ifdef ECM_DEBUG
		unsigned char pokeECMResponse[4] = {0xF4, 0x56, 0x00, 0xB6};
	#else
		unsigned char pokeECMResponse[4] = {0};
	#endif

	while ( *(uint32_t*)pokeECMResponse != POKE_ECM_RESPONSE_FAST )
	{
		funDigitalWrite(PA1, FUN_LOW);
			ALDL_UART.write(pokeECMCmd, sizeof(pokeECMCmd));
		funDigitalWrite(PA1, FUN_HIGH);

		// wait for 50ms
		simpleTimer tmr50ms(50UL);
		while (!tmr50ms.ready()) {}

		ALDL_UART.fillBuff(pokeECMResponse);
	}
}

// ****************************************************************************************

void getADLDData(void)
{
	// Here we get ALDL data
	funDigitalWrite(PA1, FUN_LOW);
		ALDL_UART.write(getECMDataCmd, sizeof(getECMDataCmd));
	funDigitalWrite(PA1, FUN_HIGH);

	// wait for 50ms
	simpleTimer tmr50ms(50UL);
	while (!tmr50ms.ready()) {}

	#ifndef ECM_DEBUG
		ALDL_UART.fillBuff((uint8_t*)&ALDLData);
	#endif
}

// ****************************************************************************************

void makeScreen(uint8_t x, uint8_t y, const unsigned char *input, uint8_t width, uint8_t height)
{
	OLEDScreen.setbuf(0);
	OLEDScreen.drawFrame(1);
	if((int)input != 0)
		OLEDScreen.drawImage(x, y, input, width, height, 0);
}

// ****************************************************************************************

void makeStartScreen(void)
{
	OLEDScreen.drawImage(10, 1, vette_logo, 88, 64, 0);
    OLEDScreen.drawchar(6, 15, 'P', 1);
    OLEDScreen.drawchar(6, 25, 'r', 1);
    OLEDScreen.drawchar(6, 35, 'e', 1);
    OLEDScreen.drawchar(6, 45, 's', 1);
    OLEDScreen.drawchar(6, 55, 's', 1);

    OLEDScreen.drawchar(116, lineNumbers[0], 'B', 1);
    OLEDScreen.drawchar(116, lineNumbers[1], 'u', 1);
    OLEDScreen.drawchar(116, lineNumbers[2], 't', 1);
    OLEDScreen.drawchar(116, lineNumbers[3], 't', 1);
    OLEDScreen.drawchar(116, lineNumbers[4], 'o', 1);
    OLEDScreen.drawchar(116, lineNumbers[5], 'n', 1);
    OLEDScreen.drawFrame(1);
}