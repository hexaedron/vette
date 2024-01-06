#include "ch32v003fun.h"
#include "extralibs/ch32v003_GPIO_branchless.h"

#include "include/gpio_pins.h"

#include <stdbool.h>
#include <string.h>    // strlen
#include <stdlib.h>    // itoa

#define PACKED __attribute__ ((__packed__))
#include "include/A172ALDL.h"
#include "include/ABSALDL.h"
#include "include/UART.h"


void system_initSystick(void);
bool system_isSysTick(void);

int main()
{
	SystemInit();
	system_initSystick();
	UART myUART(115200);
	
	uint8_t i = 20;

	__enable_irq();
	while (true)
	{
		
		if (system_isSysTick())
		{
			myUART.write(getABSMessage(i));
			i++;
			//while(myUART.available())
			//{
			//	myUART.write(myUART.read());
			//}
			if(i > 83) {i=20;}
			myUART.write("\r\n");
		}
	}
}
