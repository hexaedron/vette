#include <ch32v003fun.h>
#include <stdbool.h>

#include "include/cube_defs.h"

static volatile uint32_t _millis  = 0ULL;  

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
  NVIC_EnableIRQ(SysTicK_IRQn);

  SysTick->SR   = 0;
  SysTick->CMP  = (FUNCONF_SYSTEM_CORE_CLOCK / 1000 - 1);  // 1 ms
  SysTick->CNT  = 0; 
  SysTick->CTLR |= STK_CTRL_STRE | STK_CTRL_STE | STK_CTRL_STIE | STK_CTRL_STCK;
}

// Arduino-like millis()
uint32_t millis(void)
{
  uint32_t tmp;

  NVIC_DisableIRQ(SysTicK_IRQn);
  {
    tmp = _millis;
  }
  NVIC_EnableIRQ(SysTicK_IRQn);

  return tmp;
}


// millis() which flushes the counter for further use in uint64_t counters
void millisFlush(void)
{
  __disable_irq();
  {
    _millis = 0UL;
  }
  __enable_irq();
}

/**
*   Systick interrupt handler. It only counts millis.
*/
extern "C" __attribute__((interrupt("WCH-Interrupt-fast")))
void SysTick_Handler(void)
{ 
  _millis++;
  SysTick->SR = 0;
}


