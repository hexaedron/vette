
#include <ch32v003fun.h>
#include <stdbool.h>

#include "include/cube_defs.h"


static volatile bool _systick;
static volatile uint32_t secDiv10 = 0;


extern "C" __attribute__((interrupt("WCH-Interrupt-fast")))
void NMI_Handler(void)
{
}


extern "C" __attribute__((interrupt("WCH-Interrupt-fast")))
void HardFault_Handler(void)
{
  while (true)
  {
  }
}

void system_initSystick(void)
{
  _systick = false;

  NVIC_EnableIRQ(SysTicK_IRQn);

  SysTick->SR   = 0;
  SysTick->CMP  = (FUNCONF_SYSTEM_CORE_CLOCK / 10 - 1);  // 100 ms
  SysTick->CNT  = 0; 
  SysTick->CTLR |= STK_CTRL_STRE | STK_CTRL_STE | STK_CTRL_STIE | STK_CTRL_STCK;
}

// Use this to measure time in 100 ms intervals
uint32_t getSecDiv10(void)
{
  return secDiv10;
}

/**
*
*/
bool system_isSysTick(void)
{
   bool tmp;

  __disable_irq();
  {
    tmp = _systick;
    _systick = false;
  }
  __enable_irq();

  return tmp;
}


/**
*   Обработчик прерывания от системного таймера
*/
extern "C" __attribute__((interrupt("WCH-Interrupt-fast")))
void SysTick_Handler(void)
{
  _systick = true;
  SysTick->SR = 0;
  secDiv10++;
}


