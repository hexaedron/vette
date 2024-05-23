#include <ch32v003fun.h>
#include <stdbool.h>

#include "include/cube_defs.h"

//Globals
static volatile uint64_t     _millis       = 0ULL;    // Millisecons counter
static volatile uint32_t     _btn_millis   = 0;       // Button millisecons counter
static volatile uint32_t     _pin_num      = 0;       // Pins bitmask for EXTI
static volatile bool         _keyPressed   = false;   // Button pressed flag  
static volatile bool         _keyHeld      = false;   // Button held flag
static volatile bool         _keyRaw       = false;   // Raw key read status

#define BUTTON_DEBOUNCE_MS       10
#define BUTTON_TICK_MS           10
#define BUTTON_HOLD_TIMEOUT_MS 1000 

void system_initSystick(void)
{
  NVIC_EnableIRQ(SysTicK_IRQn);

  SysTick->SR   = 0;
  SysTick->CMP  = (FUNCONF_SYSTEM_CORE_CLOCK / 1000 - 1);  // 1 ms
  SysTick->CNT  = 0; 
  SysTick->CTLR |= STK_CTRL_STRE | STK_CTRL_STE | STK_CTRL_STIE | STK_CTRL_STCK;
}

void system_initEXTI(uint32_t pin, bool risingEdge = true, bool fallingEdge = false)
{ 
  // Setup pin-change-interrupt.  This will trigger when the voltage on the
  // pin rises above the  schmitt trigger threshold.
  AFIO->EXTICR = HW_PORT_NUM(pin) << (HW_PIN_NUM(pin) * 2);
  EXTI->INTENR = 1 << HW_PIN_NUM(pin);     // Enable the interrupt request signal for external interrupt channel
  
  if(risingEdge)  EXTI->RTENR = 1 << HW_PIN_NUM(pin);     // Rising edge trigger
  if(fallingEdge) EXTI->FTENR = 1 << HW_PIN_NUM(pin);     // Falling edge trigger
  
  _pin_num |= 1 << HW_PIN_NUM(pin); // Set the state of interrupt mask

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

// Arduino-like millis(). Does not require interrupt masking
uint32_t millis32(void)
{
  return _millis;
}

void delay_ms(uint32_t delay)
{
  uint32_t tmp = millis32();
  while (millis32() < tmp + delay)
  {
  }
}

void keyTick()
{
    static uint32_t keyTemp = 0;

    if(_btn_millis < BUTTON_TICK_MS) return;
    _btn_millis = 0;
    
    if (!_keyRaw)           // Button pressed
    {
        ++keyTemp;    
    }
    else                    // Button released  
    {
        if (keyTemp >= (BUTTON_HOLD_TIMEOUT_MS / BUTTON_TICK_MS))        
        {
            _keyHeld = true;        
        }
        else        
        {
            if (keyTemp > (BUTTON_DEBOUNCE_MS / BUTTON_TICK_MS)) _keyPressed = true;        
        }
        keyTemp = 0;    
    }
}

bool btnClick(void)
{
  if(_keyPressed)
  {
    _keyPressed = false;
    return true;
  }
  return false;
}

bool btnHeld(void)
{
  if(_keyHeld)
  {
    _keyHeld = false;
    return true;
  }
  return false;
}

/**
*   Systick interrupt handler. It only counts millis.
*/
extern "C" INTERRUPT_HANDLER
__attribute__((section(".srodata"))) 
void SysTick_Handler(void)
{ 
  _millis++;
  _btn_millis++;
  _keyRaw = funDigitalRead(PC6);
  SysTick->SR = 0;
}

//extern "C" INTERRUPT_HANDLER 
//void EXTI7_0_IRQHandler(void) 
//{
//  uint32_t pinState = funDigitalRead(PC6);
//
//  if(pinState == 1)
//  {
//    _btn_released = true;
//  }
//  else
//  {
//    _btn_pressed = true;
//  }
//
//  EXTI->INTFR = _pin_num; // Acknowledge the only interrupt we use in our init functions
//}