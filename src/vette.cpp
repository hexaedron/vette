#include "ch32v003fun.h"

#include "include/gpio_pins.h"

#include <stdbool.h>
#include <string.h>    // strlen
#include <stdlib.h>    // itoa
#include <stdio.h>	   // printf

#define PACKED __attribute__ ((__packed__))
#include "include/ALDL_cmd.h"
#include "include/A172ALDL.h"
#include "include/ABSALDL.h"

#include "include/FSM.h"

// from system.cpp
void system_initSystick(void);
void system_initEXTI(int portno, int pin, bool risingEdge = true, bool fallingEdge = false);
bool btnPressed(void);

int main()
{
	SystemInit();	
	system_initSystick();	
	funGpioInitAll();
	funPinMode(PC6, GPIO_Speed_In | GPIO_CNF_IN_PUPD);
	system_initEXTI(GPIO_PortSourceGPIOC, 6); //PC6 

	fsm_init();
	
	while (true)
	{		
		fsm_update();
	}
}
