#include <ch32v003fun.h>
#include <stdbool.h>

#include "include/cube_defs.h"


//static volatile bool _systick;
static volatile uint32_t _millis = 0;


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
  //_systick = false;

  NVIC_EnableIRQ(SysTicK_IRQn);

  SysTick->SR   = 0;
  SysTick->CMP  = (FUNCONF_SYSTEM_CORE_CLOCK / 1000 - 1);  // 1 ms
  SysTick->CNT  = 0; 
  SysTick->CTLR |= STK_CTRL_STRE | STK_CTRL_STE | STK_CTRL_STIE | STK_CTRL_STCK;
}

// Arduino-like millis()
volatile uint32_t millis(void)
{
  uint32_t tmp;

   __disable_irq();
  {
    tmp = _millis;
  }
  __enable_irq();

  return (uint32_t)tmp;
}

/**
*
*/
//bool system_isSysTick(void)
//{
//   bool tmp;
//
//  __disable_irq();
//  {
//    tmp = _systick;
//    _systick = false;
//  }
//  __enable_irq();
//
//  return tmp;
//}


/**
*   Systick interrupt handler. It only counts millis.
*/
extern "C" __attribute__((interrupt("WCH-Interrupt-fast")))
void SysTick_Handler(void)
{
  //_systick = true;
  _millis++;
  SysTick->SR = 0;
}


