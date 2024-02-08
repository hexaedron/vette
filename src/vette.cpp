#include "ch32v003fun.h"

#include "include/gpio_pins.h"

#include <stdbool.h>
#include <string.h>    // strlen
#include <stdlib.h>    // itoa
#include <stdio.h>	   // printf

#include "sh1106.h"


#define PACKED __attribute__ ((__packed__))
#include "include/ALDL_cmd.h"
#include "include/A172ALDL.h"
#include "include/ABSALDL.h"

#include "include/UART.h"
#include "include/tim2Encoder.h"

#include "include/FSM.h"

// from system.cpp
void system_initSystick(void);
bool system_isSysTick(void);
void system_initEXTI(int portno, int pin, bool risingEdge = true, bool fallingEdge = false);
bool btnPressed(void);

#include "include/simpleTimer.h"

int main()
{
	SystemInit();	
	system_initSystick();	
	funGpioInitAll();
	funPinMode(PC6, GPIO_Speed_In | GPIO_CNF_IN_PUPD);
	system_initEXTI(GPIO_PortSourceGPIOC, 6); //PC6 


	UART myUART;
	sh1106 OLEDScreen;
	tim2Encoder enc(AFIO_PCFR1_TIM2_REMAP_NOREMAP);
	simpleTimer tmr(1000UL);

	myUART.beginHD(8192);
	OLEDScreen.init();
	fsm_init();

	
	OLEDScreen.drawFrame(1);

	
	while (true)
	{		
		fsm_update();
	}
}
