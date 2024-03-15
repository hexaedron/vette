#pragma once

void fsm_init();
void fsm_update();
void fsm_init_state ();
void fsm_drawECMErrors_state ();
void fsm_drawECMParametersTemp_state ();
void fsm_drawABSParameters_state ();
void fsm_drawECMParametersRPM_state ();
void fsm_drawECMParametersBLM_state();
void fsm_drawECMParametersPressure_state ();
void fsm_drawECMParametersVoltage_state ();
void fsm_drawABSErrors_state ();
void fsm_drawFanStatus_state ();
void fsm_connectECM_state();

void waitForECMSync(void);
void getADLDData(void);
void getABSData(void);
void makeScreen(uint8_t x, uint8_t y, const unsigned char *input, uint8_t width, uint8_t height);
void makeStartScreen(void);
void makeVersionScreen(void);
void CLS(void);
char* getPaddedSpeed(uint8_t);