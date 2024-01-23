#include <ch32v003fun.h>
#include <stdbool.h>

#include "include/cube_defs.h"

static volatile uint64_t _millis  = 0ULL;   

extern "C" __attribute__((interrupt))
void NMI_Handler(void)
{
}


extern "C" __attribute__((interrupt))
void HardFault_Handler(void)
{
  while (true)
  {
  }
}

void system_initSystick(void)
{
  NVIC_EnableIRQ(SysTicK_IRQn);

  SysTick->SR   = 0;
  SysTick->CMP  = (FUNCONF_SYSTEM_CORE_CLOCK / 1000 - 1);  // 1 ms
  SysTick->CNT  = 0; 
  SysTick->CTLR |= STK_CTRL_STRE | STK_CTRL_STE | STK_CTRL_STIE | STK_CTRL_STCK;
}

// Arduino-like millis()
uint64_t millis(void)
{
  uint64_t tmp;

  NVIC_DisableIRQ(SysTicK_IRQn);
  {
    tmp = _millis;
  }
  NVIC_EnableIRQ(SysTicK_IRQn);

  return tmp;
}

/**
*   Systick interrupt handler. It only counts millis.
*/
extern "C" __attribute__((interrupt)) __attribute__((section(".srodata")))
void SysTick_Handler(void)
{ 
  _millis++;
  SysTick->SR = 0;
}
