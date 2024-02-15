#pragma once

void fsm_init();
void fsm_update();
void fsm_init_state ();
void fsm_drawECUErrors_state ();
void fsm_drawECUParameters1_state ();
void fsm_drawABSParameters_state ();
void fsm_drawECUParameters2_state ();
void fsm_drawABSErrors_state ();
void fsm_drawFanStatus_state ();
void fsm_connectECU_state();

void waitForECMSync(void);
void getADLDData(void);
void makeScreen(uint8_t x, uint8_t y, const unsigned char *input, uint8_t width, uint8_t height);
void makeStartScreen(void);