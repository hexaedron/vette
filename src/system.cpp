#include <ch32v003fun.h>
#include <stdbool.h>

#include "include/cube_defs.h"
#include "include/u64Class.h"

static volatile uint32_t _millis  = 0UL;  
static  volatile uint32_t _millis_hi  = 0UL;  
static  volatile uint32_t _millis_lo  = 0UL;  

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

//extern "C" 
//__attribute__((optimize("O0"))) 
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

//#pragma GCC push_options
//#pragma GCC reset_options
//#pragma GCC optimize "Os"
//#pragma GCC optimize "no-forward-propagate"
//#pragma GCC optimize "no-move-loop-invariants"
//#pragma GCC optimize "no-inline-small-funtions"
u64 millis64(void)
{
  static volatile uint32_t tmp_lo;
  static volatile uint32_t tmp_hi;

  NVIC_DisableIRQ(SysTicK_IRQn);
  {
    tmp_lo = _millis_lo;
    tmp_hi = _millis_hi;
  }
  NVIC_EnableIRQ(SysTicK_IRQn);

  u64 ret(tmp_lo, tmp_hi);
  return ret;
}
//#pragma GCC pop_options


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
extern "C" __attribute__((interrupt))
void SysTick_Handler(void)
{ 
  if(_millis_lo != __UINT32_MAX__)
  {
      _millis_lo++;
  }
  else
  {
      _millis_lo = 0UL;
      _millis_hi++; 
  }
  
  _millis++;
  SysTick->SR = 0;
}
