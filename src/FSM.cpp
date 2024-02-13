#include "ch32v003fun.h"
#include "include/FSM.h"
#include "include/tim2Encoder.h"
#include "include/simpleTimer.h"
#include "sh1106.h"
#include "include/UART.h"

#include "include/bitmaps.h"
#include "include/line_nums.h"

// from system.cpp
bool btnPressed(uint32_t);

tim2Encoder enc(AFIO_PCFR1_TIM2_REMAP_NOREMAP);
simpleTimer tmr(1000UL);
sh1106 OLEDScreen;
UART myUART;

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

void fsm_init_state()
{
	// Declare local/static variable here.

	if ( fsm_enter_state_flag )
	{
		// Run once when enter this state.
        myUART.beginHD(8192);
        OLEDScreen.init();
        OLEDScreen.drawImage(10, 1, vette_logo, 88, 64, 0);
        OLEDScreen.drawchar(6, 15, 'P', 1);
        OLEDScreen.drawchar(6, 25, 'r', 1);
        OLEDScreen.drawchar(6, 35, 'e', 1);
        OLEDScreen.drawchar(6, 45, 's', 1);
        OLEDScreen.drawchar(6, 55, 's', 1);

        OLEDScreen.drawchar(116, line_1, 'B', 1);
        OLEDScreen.drawchar(116, line_2, 'u', 1);
        OLEDScreen.drawchar(116, line_3, 't', 1);
        OLEDScreen.drawchar(116, line_4, 't', 1);
        OLEDScreen.drawchar(116, line_5, 'o', 1);
        OLEDScreen.drawchar(116, line_6, 'n', 1);
        OLEDScreen.drawFrame(1);
        OLEDScreen.refresh();
	}
	// Run repeatly for update.


	if ( btnPressed(PC6) )
	{
		fsm_state = &fsm_drawECUErrors_state;
		fsm_enter_state_flag = true;
		return;
	}

	fsm_enter_state_flag = false; // Reset flag
}

void fsm_drawECUErrors_state()
{
	// Declare local/static variable here.

	if ( fsm_enter_state_flag )
	{
		// Run once when enter this state.
		OLEDScreen.setbuf(0);
		OLEDScreen.drawFrame(1);
		OLEDScreen.drawImage(87, 0, errors_bitmap, 32, 8, 0);
		OLEDScreen.refresh();
	}
	// Run repeatly for update.


	if ( btnPressed(PC6) )
	{
		fsm_state = &fsm_drawECUParameters1_state;
		fsm_enter_state_flag = true;
		return;
	}
	fsm_enter_state_flag = false; // Reset flag
}

void fsm_drawECUParameters1_state()
{
	// Declare local/static variable here.

	if ( fsm_enter_state_flag )
	{
		// Run once when enter this state.
		OLEDScreen.setbuf(0);
		OLEDScreen.drawFrame(1);
		OLEDScreen.drawImage(87, 0, params_bitmap, 32, 8, 0);
		OLEDScreen.refresh();
	}
	// Run repeatly for update.


	if ( btnPressed(PC6)  )
	{
		fsm_state = &fsm_drawECUParameters2_state;
		fsm_enter_state_flag = true;
		return;
	}
	fsm_enter_state_flag = false; // Reset flag
}

void fsm_drawABSParameters_state()
{
	// Declare local/static variable here.

	if ( fsm_enter_state_flag )
	{
		// Run once when enter this state.
		OLEDScreen.setbuf(0);
		OLEDScreen.drawFrame(1);
		OLEDScreen.drawImage(94, 0, abs_bitmap, 16, 8, 0);
		OLEDScreen.refresh();
	}
	// Run repeatly for update.


	if ( btnPressed(PC6) )
	{
		fsm_state = &fsm_drawECUErrors_state;
		fsm_enter_state_flag = true;
		return;
	}
	fsm_enter_state_flag = false; // Reset flag
}

void fsm_drawECUParameters2_state()
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
		OLEDScreen.setbuf(0);
		OLEDScreen.drawFrame(1);
		OLEDScreen.drawImage(84, 0, abs_err_bitmap, 32, 8, 0);
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

void fsm_drawFanStatus_state()
{
	// Declare local/static variable here.

	if ( fsm_enter_state_flag )
	{
		// Run once when enter this state.
		OLEDScreen.setbuf(0);
		OLEDScreen.drawFrame(1);
		OLEDScreen.drawImage(94, 0, fan_bitmap, 16, 8, 0);
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
