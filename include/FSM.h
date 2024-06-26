#pragma once

#include "include/cube_defs.h"
#include "include/tim2Encoder.h"
#include "sh1106.h"
#include "include/bitmaps.h"
#include "include/line_nums.h"

#include "include/A172ALDL.h"
#include "include/ABSALDL.h"

void fsm_init();
void fsm_update();
void fsm_init_state ();
void fsm_selectMode_state();
void fsm_instructionsForABS_state();
void fsm_drawECMErrors_state ();
void fsm_resetECMErrors_state();
void fsm_drawECMParametersTemp_state ();
void fsm_drawABSParameters_state ();
void fsm_drawECMParametersRPM_state ();
void fsm_drawECMParametersBLM_state();
void fsm_drawECMParametersPressure_state ();
void fsm_drawECMParametersVoltage_state ();
void fsm_drawABSErrors_state ();
void fsm_drawFanStatus_state ();
void fsm_connectECM_state();

void getADLDData(void);
void flushADLDErrors(void);
void getABSData(void);
void makeScreen(uint8_t x, uint8_t y, const unsigned char *input, uint8_t width, uint8_t height);
void makeStartScreen(void);
void makeVersionScreen(void);
void CLS(void);
char* getPaddedSpeed(uint8_t);

// from system.cpp
bool btnClick(void);
bool btnHeld(void);
void keyTick(void);
void delay_ms(uint32_t);

//#define ECM_DEBUG
#define NEED_SILENT_MODE

#define ALDL_MESSAGE_MS 85
#define ABS_MESSAGE_MS 50
#define ALDL_POLL_MS 200
#define ABS_SILENT_MESSAGE_MS 10

#ifdef NEED_SILENT_MODE
    #define SILENT_MESSAGE_MS 10
#else 
    #define SILENT_MESSAGE_MS 0
#endif

#define ENABLE_UART_WRITE()   funDigitalWrite(PA1, FUN_LOW)
#define DISABLE_UART_WRITE() funDigitalWrite(PA1, FUN_HIGH)