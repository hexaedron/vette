#include <ch32v003fun.h>
#include <stdbool.h>

#include "include/cube_defs.h"

//Globals
static volatile uint64_t     _millis  = 0ULL;    // Millisecons counter
static volatile bool         _btn     = false;   // Button pressed flag
static volatile uint32_t     _pin_num = 0;       // Pins bitmask for EXTI

#define BUTTON_DEBOUNCE_MS 50ULL

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

void system_initEXTI(int portno, int pin, bool risingEdge = true, bool fallingEdge = false)
{ 
  // Setup pin-change-interrupt.  This will trigger when the voltage on the
  // pin rises above the  schmitt trigger threshold.
  AFIO->EXTICR = portno << (pin * 2);
  EXTI->INTENR = 1 << pin;     // Enable the interrupt request signal for external interrupt channel
  
  if(risingEdge)  EXTI->RTENR = 1 << pin;     // Rising edge trigger
  if(fallingEdge) EXTI->FTENR = 1 << pin;     // Falling edge trigger
  
  _pin_num |= 1 << pin; // Set the state of interrupt mask

  NVIC_EnableIRQ(EXTI7_0_IRQn);
}

// Arduino-like millis()
uint64_t millis(void)
{
  uint64_t tmp;

  // critical section
  NVIC_DisableIRQ(SysTicK_IRQn);
  {
    tmp = _millis;
  }
  NVIC_EnableIRQ(SysTicK_IRQn);

  return tmp;
}

bool btnPressed(uint32_t pin)
{
  static volatile uint64_t lastPressed = 0ULL;
  
  // Disable IRQ for the given pin. 
  // We will enable it back later for debouncing
  EXTI->INTENR &= ~(1 << (pin & 0xF ));
 
  if( _btn )
  {
    _btn = false;
   
    if ( (millis() - lastPressed) >= BUTTON_DEBOUNCE_MS )
    {
      lastPressed = millis();
      // Enable IRQ for the given pin. 
      EXTI->INTENR |= (1 << (pin & 0xF ));
      return true; 
    }
    else
    {
      return false;
    }
  }
  else 
  {
    // Enable IRQ for the given pin. 
    EXTI->INTENR |= (1 << (pin & 0xF ));
    return false;
  }
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

extern "C" __attribute__((interrupt))
void EXTI7_0_IRQHandler(void) 
{
  _btn = true;
  EXTI->INTFR = _pin_num; // Acknowledge the only interrupt we use in our init functions
}