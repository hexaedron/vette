#include "ch32v003fun.h"
#include "include/cube_defs.h"

#include "include/FSM.h"
#include "include/tim2Encoder.h"
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

//#define ECM_DEBUG

#define BUTTON_HOLD_TIMEOUT_MS 1000UL

extern const char vette_version[];

// from system.cpp
bool btnClick(uint32_t);
bool btnHeld(uint32_t, uint32_t);
void delay_ms(uint32_t);

tim2Encoder enc(AFIO_PCFR1_TIM2_REMAP_NOREMAP);
sh1106 OLEDScreen;
UART ALDL_UART;
A172ALDL ALDLData = {0};
ABSALDL ABSData   = {0};
ALDLParser myALDLParser;

// https://menginventor.github.io/FSM_coder/#


// This section generated by FSM Coder
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

		// Prepare parser. We should attach only once.
		myALDLParser.attach(&ALDLData);
	}
	
	// Run repeatly for update.

	// Version info etc.
	int8_t delta = enc.getDelta();
	if( delta != 0 )
	{
		makeVersionScreen();
	}


	if ( btnClick(PC6) )
	{
		fsm_state = &fsm_connectECM_state;
		fsm_enter_state_flag = true;
		return;
	}

	fsm_enter_state_flag = false; // Reset flag
}

// ****************************************************************************************

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
		//*********************************************
		//************** EXPERIMENTAL!!! **************
		//*********************************************
		while (!myALDLParser.validateChecksum())
		{
			getADLDData();
		}

		

		#ifdef ECM_DEBUG
			ALDLData.MALFFLG1 = 0xFF;
			ALDLData.MALFFLG1 = 0xFF;
			ALDLData.MALFFLG3 = 0xFF;
			ALDLData.MALFFLG4 = 0b11010111;
			ALDLData.MALFFLG5 = 0xFF;
		#endif
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


	//if ( btnHeld(PC6, BUTTON_HOLD_TIMEOUT_MS) )
	//{
	//	fsm_state = &fsm_resetECMErrors_state;
	//	fsm_enter_state_flag = true;
	//	return;
	//}

	if ( btnClick(PC6) )
	{
		fsm_state = &fsm_drawECMParametersTemp_state;
		fsm_enter_state_flag = true;
		return;
	}
	fsm_enter_state_flag = false; // Reset flag
}

// ****************************************************************************************

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


	if ( btnClick(PC6)  )
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

// ****************************************************************************************

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

	getADLDData();
	//*********************************************
	//************** EXPERIMENTAL!!! **************
	//*********************************************
	if(!myALDLParser.validateChecksum())
	{
		fsm_enter_state_flag = false;
		return;
	}
	
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


	if ( btnClick(PC6)  )
	{
		fsm_state = &fsm_drawECMParametersRPM_state;
		fsm_enter_state_flag = true;
		return;
	}
	fsm_enter_state_flag = false; // Reset flag
}

// ****************************************************************************************

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

	getADLDData();
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


	if ( btnClick(PC6) )
	{
		fsm_state = &fsm_drawECMParametersBLM_state;
		fsm_enter_state_flag = true;
		return;
	}
	fsm_enter_state_flag = false; // Reset flag
}
// ****************************************************************************************

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

	getADLDData();
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


	if ( btnClick(PC6) )
	{
		fsm_state = &fsm_drawECMParametersPressure_state;
		fsm_enter_state_flag = true;
		return;
	}
	fsm_enter_state_flag = false; // Reset flag
}

// ****************************************************************************************

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

	getADLDData();
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


	if ( btnClick(PC6) )
	{
		fsm_state = &fsm_drawECMParametersVoltage_state;
		fsm_enter_state_flag = true;
		return;
	}
	fsm_enter_state_flag = false; // Reset flag
}

// ****************************************************************************************

void fsm_drawECMParametersVoltage_state()
{
	// Declare local/static variable here.

	if ( fsm_enter_state_flag )
	{
		// Run once when enter this state.
		makeScreen(87, 0, params_bitmap, 32, 8);
	}
	
	// Run repeatly for update.
	
	#ifdef ECM_DEBUG
		ALDLData.ADBAT = 131;
	#endif

	getADLDData();
	CLS();

	
	// Here we print voltage
	uint8_t printPos;

	OLEDScreen.drawstr(35, lineNumbers[1], (char*)"Voltage:", 1);
	printPos = (138 - (strlen(myALDLParser.getVoltage()) + 1) * 15) / 2;
	OLEDScreen.drawstr_sz(printPos, lineNumbers[4], myALDLParser.getVoltage(), 1, fontsize_15x24);

	OLEDScreen.refresh();


	if ( btnClick(PC6)  )
	{
		fsm_state = &fsm_drawFanStatus_state;
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
		OLEDScreen.drawImage(12, lineNumbers[0] - 6, fan_propeller_bitmap, 24, 24, 0);
		OLEDScreen.drawImage(76, lineNumbers[0] - 6, fan_propeller_bitmap, 24, 24, 0);
		OLEDScreen.drawchar(42,  lineNumbers[1] - 2, '1', 1);
		OLEDScreen.drawchar(108, lineNumbers[1] - 2, '2', 1);
	}
	
	// Run repeatly for update.
	#ifdef ECM_DEBUG
		ALDLData.FANMW  = 0b01111111;
	#endif

	getADLDData();

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

	if ( btnClick(PC6) )
	{
		fsm_state = &fsm_drawABSErrors_state;
		fsm_enter_state_flag = true;
		return;
	}
	fsm_enter_state_flag = false; // Reset flag
}

// ****************************************************************************************

void fsm_drawABSErrors_state()
{
	// Declare local/static variable here.

	if ( fsm_enter_state_flag )
	{
		// Run once when enter this state.
		getABSData();
		makeScreen(84, 0, abs_err_bitmap, 32, 8);

		#ifdef ECM_DEBUG
			ABSData.fc1.faultCodeNum = 66;
			ABSData.fc2.faultCodeNum = 41;
			ABSData.fc3.faultCodeNum = 74;
		#endif

		if( (ABSData.fc1.faultCodeNum == 0xFF) && (ABSData.fc2.faultCodeNum == 0xFF) && (ABSData.fc3.faultCodeNum == 0xFF))
		{
			OLEDScreen.drawstr_sz(8, lineNumbers[3], (char*)"No errors!", 1, fontsize_10x16);
		}
		else
		{
			OLEDScreen.drawstr(4, lineNumbers[1] - 4, (char*)getABSMessage(ABSData.fc1.faultCodeNum), 1);
			OLEDScreen.drawstr(4, lineNumbers[3] - 4, (char*)getABSMessage(ABSData.fc2.faultCodeNum), 1);
			OLEDScreen.drawstr(4, lineNumbers[5] - 4, (char*)getABSMessage(ABSData.fc3.faultCodeNum), 1);
		}
		
		OLEDScreen.refresh();
	}

	// Run repeatly for update.

	if ( btnClick(PC6) )
	{
		fsm_state = &fsm_drawABSParameters_state;
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
	}
	
	// Run repeatly for update.

	#ifdef ECM_DEBUG
		ABSData.LFWheelSpeed = 47;
		ABSData.LRWheelSpeed = 47;
		ABSData.RFWheelSpeed = 47;
		ABSData.RRWheelSpeed = 47;
	#endif

	getABSData();

	OLEDScreen.drawstr(80, lineNumbers[0] + 3, getPaddedSpeed(ABSData.LFWheelSpeed), 1);
	OLEDScreen.drawstr(30, lineNumbers[0] + 3, getPaddedSpeed(ABSData.LRWheelSpeed), 1);
	OLEDScreen.drawstr(30, lineNumbers[5] - 3, getPaddedSpeed(ABSData.RRWheelSpeed), 1);
	OLEDScreen.drawstr(80, lineNumbers[5] - 3, getPaddedSpeed(ABSData.RFWheelSpeed), 1);

	OLEDScreen.refresh();	

	if ( btnClick(PC6) )
	{
		fsm_state = &fsm_drawECMErrors_state;
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
			ALDL_UART.flush();
		funDigitalWrite(PA1, FUN_HIGH);

		// wait for 50ms
		delay_ms(50);

		ALDL_UART.fillBuff(pokeECMResponse, sizeof(pokeECMResponse));
	}
}

// ****************************************************************************************

void getADLDData(void)
{
	memset(&ALDLData, 0, sizeof(ALDLData));

	// Here we get ALDL data
	funDigitalWrite(PA1, FUN_LOW);
		ALDL_UART.write(getECMDataCmd, sizeof(getECMDataCmd));
		// Flush first to ensure there is nothing left in buffer
		ALDL_UART.flush();
	funDigitalWrite(PA1, FUN_HIGH);

	// wait for 90ms to ensure we get all the data
	delay_ms(85);

	#ifndef ECM_DEBUG
		ALDL_UART.fillBuff((uint8_t*)&ALDLData, sizeof(ALDLData));
	#endif

	delay_ms(415);

}

// ****************************************************************************************

void flushADLDErrors()
{
	// Here we clear ALDL errors
	funDigitalWrite(PA1, FUN_LOW);
		ALDL_UART.write(clearCodesCmd, sizeof(clearCodesCmd));
	funDigitalWrite(PA1, FUN_HIGH);

	// wait for 500ms
	delay_ms(500);
}

// ****************************************************************************************

void getABSData(void)
{
	memset(&ABSData, 0, sizeof(ABSData));

	// Here we get ALDL data
	funDigitalWrite(PA1, FUN_LOW);
		ALDL_UART.write(silentModeCmd, sizeof(silentModeCmd)); // Set silent mode
		delay_ms(20);
		ALDL_UART.flush();	
		ALDL_UART.write(getABSDataCmd, sizeof(getABSDataCmd)); // Get data
	funDigitalWrite(PA1, FUN_HIGH);

	// wait for 50ms to ensure we get all the data
	delay_ms(50);

	ABSData.fc1.faultCodeNum = 0xFF;
	ABSData.fc2.faultCodeNum = 0xFF;
	ABSData.fc3.faultCodeNum = 0xFF;

	#ifndef ECM_DEBUG
		ALDL_UART.fillBuff((uint8_t*)&ABSData, sizeof(ABSData));
	#endif

	// Return to normal mode
	funDigitalWrite(PA1, FUN_LOW);
		ALDL_UART.write(returnFromABSCmd, sizeof(returnFromABSCmd));
	funDigitalWrite(PA1, FUN_HIGH);

	delay_ms(450);
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

// ****************************************************************************************

void makeVersionScreen(void)
{
	// Clear screen
	OLEDScreen.setbuf(0);
	OLEDScreen.drawFrame(1);

	OLEDScreen.drawstr(4,  lineNumbers[1] - 4, (char*)"Version:",    1);
	OLEDScreen.drawstr(56, lineNumbers[1] - 4, (char*)vette_version, 1);
	OLEDScreen.drawstr(4,  lineNumbers[3] - 4, (char*)"Date:",       1);
	OLEDScreen.drawstr(46, lineNumbers[3] - 4, (char*)__DATE__,      1);
	OLEDScreen.drawstr(4,  lineNumbers[5] - 4, (char*)"GCC",         1);

	char gcc_ver[10];
	char buf[4];
	itoa(__GNUC__, gcc_ver, 10);
	strcat(gcc_ver, ".");
	itoa(__GNUC_MINOR__, buf, 10);
	strcat(gcc_ver, buf);
	strcat(gcc_ver, ".");
	itoa(__GNUC_PATCHLEVEL__, buf, 10);
	strcat(gcc_ver, buf);

	OLEDScreen.drawstr(30, lineNumbers[5] - 4, gcc_ver,      1);

	OLEDScreen.refresh();
}

// ****************************************************************************************

inline void CLS(void)
{
	OLEDScreen.fillRect(2, 4, 125, 58, 0);
}

// ****************************************************************************************

char* getPaddedSpeed(uint8_t spd)
{
	static char buf[4];

	*(uint32_t*) buf = 0UL;

	itoa(spd << 1, buf, 10);
	for (uint8_t i = strlen(buf); i < 3; i++)
	{
		buf[i] = ' ';
	}

	return buf;
}