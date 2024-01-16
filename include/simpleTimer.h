#pragma once
#include <cstdint>

uint32_t millis(void);

class simpleTimer 
{
  public:
    simpleTimer();
    simpleTimer (uint32_t);
    void start_int();
    void stop();
    bool ready();

  private:
      uint32_t tmr;
      uint32_t prd;
};