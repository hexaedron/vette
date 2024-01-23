#include "ch32v003fun.h"
#include "extralibs/ch32v003_GPIO_branchless.h"

#include "include/gpio_pins.h"

#include <stdbool.h>
#include <string.h>    // strlen
#include <stdlib.h>    // itoa
#include <stdio.h>	   // printf


#define SSD1306_128X64
//#define printf(x, y) // To avoid printf() in 1306 lib
#include "include/ssd1306_i2c.h"
#include "include/ssd1306.h"

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
	Delay_Ms( 200 );	
	system_initSystick();
	
	funGpioInitAll();
	GPIOC->CFGLR = (GPIO_Speed_In | GPIO_CNF_IN_PUPD)<<(4*6); //PC6 for input w/Pull-up
	system_initEXTI(GPIO_port_C, 6); //PC6


	//if(!ssd1306_i2c_init())
	//{
	//	ssd1306_init();
	//}
	
	UART myUART;
	myUART.begin(115200);

	tim2Encoder enc;
	enc.init();

	char buf[10];

	simpleTimer tmr(1000UL);

	//ssd1306_setbuf(0);
	//ssd1306_drawstr(0,16, "This is a test", 1);

	while (true)
	{
		if (tmr.ready())
		{			
			int32_t delta = enc.getDelta();

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
