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

// from system.cpp
void system_initSystick(void);
bool system_isSysTick(void);
void system_initEXTI(int portno, int pin, bool risingEdge = true, bool fallingEdge = false);
bool btnPressed(void);

#include "include/simpleTimer.h"

int main()
{
	SystemInit();
	//Delay_Ms( 500 );	

	sh1106 OLEDScreen;
	OLEDScreen.init();
	//OLEDScreen.flipH();
	//OLEDScreen.flipV();
	
	//Delay_Ms( 500 );	
	system_initSystick();
	
	funGpioInitAll();
	funPinMode(PC6, GPIO_Speed_In | GPIO_CNF_IN_PUPD);
	system_initEXTI(GPIO_PortSourceGPIOC, 6); //PC6 


	UART myUART;
	myUART.begin(115200);

	tim2Encoder enc(AFIO_PCFR1_TIM2_REMAP_NOREMAP);

	char buf[10];

	simpleTimer tmr(1000UL);

	
	//OLEDScreen.drawstr_sz(0,32, (char*)"���� test", 1, fontsize_10x16);
	//OLEDScreen.refresh();

	funPinMode(PA1, GPIO_Speed_10MHz | GPIO_CNF_OUT_PP);
	funPinMode(PA2, GPIO_Speed_10MHz | GPIO_CNF_OUT_PP);

	bool t = false;
	while (true)
	{
		
		if (tmr.ready())
		{			
			int32_t delta = enc.getDelta();

			if(t)
			{
				OLEDScreen.drawstr_sz(0,32, (char*)"PA1 ���.", 1, fontsize_10x16);
				t = !t;
				funDigitalWrite(PA1, FUN_HIGH);
				funDigitalWrite(PA2, FUN_LOW);
			}
			else
			{
				OLEDScreen.drawstr_sz(0,32, (char*)"PA2 ���.", 1, fontsize_10x16);
				t = !t;
				funDigitalWrite(PA1, FUN_LOW);
				funDigitalWrite(PA2, FUN_HIGH);
			}

			OLEDScreen.refresh();

			if(delta < 0)
			{
				itoa(delta, buf, 10);
				myUART.write(buf);
				myUART.write("\n\r");
			}
			else if (delta > 0)
			{
				itoa(delta, buf, 10);
				myUART.write(buf);
				myUART.write("\n\r");
			}

			if(btnPressed())
			{
				myUART.write("Button pressed!\n\r");
			}
		}
	}
}
