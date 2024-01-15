#include <ch32v003fun.h>
#include <stdbool.h>

#include "include/cube_defs.h"
//#include "include/uint64Class.h"


//static volatile uint32_t _millis_low  = 0UL; 
//static volatile uint32_t _millis_high = 0UL; 
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

  __disable_irq();
  {
    tmp = _millis;
  }
  __enable_irq();

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

  //__disable_irq();
  //_millis = 0UL;
  //__enable_irq();

  //return tmp;
}

//volatile int64_manual millis(void)
//{
//  int64_manual tmp;
//  static volatile uint32_t l, h;
//
//  __disable_irq();
//  {
//    //tmp.setLo(_millis_low);
//    //tmp.setHi(_millis_high);
//    l=_millis_low;
//    h=_millis_high;
//  }
//  __enable_irq();
//
//  tmp.setLo(l);
//  tmp.setHi(h);
//  return tmp;
//}


/**
*   Systick interrupt handler. It only counts millis.
*/
extern "C" __attribute__((interrupt("WCH-Interrupt-fast")))
void SysTick_Handler(void)
{
  
  //if(_millis_low != __UINT32_MAX__)
  //{
  //    _millis_low++;
  //}
  //else
  //{
  //    _millis_low = 0UL;
  //    _millis_high++; 
  //}

  _millis++;
  SysTick->SR = 0;
}


