#include "ch32v003fun.h"
#include "ch32v003_GPIO_branchless.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "include/A172ALDL.h"
#include "include/UART.h"

#include "include/gpio_pins.h"

void system_initSystick(void);
bool system_isSysTick(void);

int main()
{
	SystemInit();
	system_initSystick();
	UART myUART(115200);

	__enable_irq();
	while (true)
	{
		if (system_isSysTick())
		{
			while(myUART.available())
			{
				myUART.write(myUART.read());
			}
			myUART.write("\r\n");
		}
	}
}
