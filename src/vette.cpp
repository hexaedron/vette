#include "ch32v003fun.h"
#include "extralibs/ch32v003_GPIO_branchless.h"

#include "include/gpio_pins.h"

#include <stdbool.h>
#include <string.h>    // strlen
#include <stdlib.h>    // itoa


#define SSD1306_128X64
#define printf(x, y) // To avoid printf() in 1306 lib
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
void millisFlush(void);
//#include "include/timeKeeperClass.h"
//timeKeeper timeKeep;

#include "include/simpleTimer.h"

int main()
{
	SystemInit();
	system_initSystick();

	UART myUART;
	myUART.begin(115200);

	char str[50] = {0};

	simpleTimer tmr10s(3000UL);

	//__enable_irq();
	while (true)
	{
		if (tmr10s.ready())
		{
			//if (system_isSysTick())
			//{
				//while(myUART.available())
				//{
				//	myUART.write(myUART.read());
				//}
				myUART.fillBuff((uint8_t*)str);
				myUART.write(str);
				memset(str, 0, 50);
				myUART.write("\n\rTick\n\r");
			//}
		}
	}
}
