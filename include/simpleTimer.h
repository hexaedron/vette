#pragma once
#include <cstdint>

#include "include/u64Class.h"

uint32_t millis(void);
u64 millis64(void);

class simpleTimer 
{
  public:
    simpleTimer();
    simpleTimer (uint32_t);
    void start_int();
    void stop();
    bool ready();

  private:
      u64 tmr;
      u64 prd;
};