#pragma once

#define STK_CTRL_STRE 0b0000000000000000000000000001000
#define STK_CTRL_STIE 0b0000000000000000000000000000010
#define STK_CTRL_STE  0b0000000000000000000000000000001
#define STK_CTRL_SWIE 0b1000000000000000000000000000000
#define STK_CTRL_STCK 0b0000000000000000000000000000100

#define HW_PORT_NUM(pin) ( (pin & 0xFFFFFFF0) >> 4 )
#define HW_PIN_NUM(pin)  (  pin & 0xF )

RV_STATIC_INLINE void EnableIRQ()
{
  uint32_t result;

    __asm volatile(
#if __GNUC__ > 10
		".option arch, +zicsr\n"
#endif
		"csrr %0," "mstatus": "=r"(result));
  result |= 0x8;
  __asm volatile ("csrw mstatus, %0" : : "r" (result) );
}

/*********************************************************************
 * @fn      __disable_irq
 *
 * @brief   Disable Global Interrupt
 *
 * @return  none
 */
RV_STATIC_INLINE void DisableIRQ()
{
  uint32_t result;

    __asm volatile(
#if __GNUC__ > 10
		".option arch, +zicsr\n"
#endif
		"csrr %0," "mstatus": "=r"(result));
  result &= ~0x8;
  __asm volatile ("csrw mstatus, %0" : : "r" (result) );
}

#define WCH_FAST_INTERRUPT_ENABLED

#ifdef WCH_FAST_INTERRUPT_ENABLED
  #define INTERRUPT_HANDLER __attribute__((interrupt("WCH-Interrupt-fast")))
#else
  #define INTERRUPT_HANDLER __attribute__((interrupt)) 
#endif

// Arduino-like bit macros
#define bit(n)              (1 << (n))
#define bitSet(x, n)        ((x) |= bit(n))
#define bitClear(x, n)      ((x) &= ~bit(n))
#define bitRead(x, n)       (((x) & bit(n)) !=0 )
#define bitWrite(x, n, b)   ((b) ? bitSet((x), (n)) : bitClear((x), (n)))