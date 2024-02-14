#include "ch32v003fun.h"

#include "include/gpio_pins.h"

#include <stdbool.h>
#include <string.h>    // strlen
#include <stdlib.h>    // itoa
#include <stdio.h>	   // printf

#include "include/ALDL_cmd.h"
#include "include/A172ALDL.h"
#include "include/ABSALDL.h"

#include "include/FSM.h"

// from system.cpp
void system_initSystick(void);
void system_initEXTI(uint32_t pin, bool risingEdge = true, bool fallingEdge = false);

int main()
{
	SystemInit();	
	system_initSystick();

	// We use button on PC6, so we need to init it and turn on interrupt.	
	funGpioInitAll();
	funPinMode(PC6, GPIO_Speed_In | GPIO_CNF_IN_FLOATING);
	system_initEXTI(PC6);  

	// We use PA1 as DIR pin for SN74LVC1T45DBVR
	// When it is low, we transmit,
	// when it is high, we receive
	funPinMode(PA1, GPIO_Speed_10MHz | GPIO_CNF_OUT_PP);

	fsm_init();
	
	while (true)
	{		
		fsm_update();
	}
}
