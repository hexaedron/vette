#include "include/simpleTimer.h"
    
simpleTimer::simpleTimer () 
{

}

simpleTimer::simpleTimer (uint32_t nprd) 
{
  prd = nprd;
  start_int();
}

void simpleTimer::start_int() 
{
  tmr = millis();
  if (tmr == 0UL) tmr = 1UL;
}

void simpleTimer::stop() 
{
  tmr = 0UL;
}

bool simpleTimer::ready() 
{
  if ( ( tmr != 0UL) && (millis() - tmr >= prd) )
  {
    start_int();
    return true;
  }
  
  return false;
}