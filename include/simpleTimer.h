#pragma once

class simpleTimer 
{
  public:

    simpleTimer () {};

    simpleTimer (uint32_t nprd) 
    {
      prd = nprd;
      start_int();
    };

    void start_int() 
    {
      tmr = millis();
      if (tmr == 0UL) tmr = 1UL;
    };

    void stop() 
    {
      tmr = 0UL;
    };

    bool ready() 
    {
      uint32_t tmp = (millis() - tmr);
      if ((tmr != 0UL) && (tmp >= prd)) 
      {
        start_int();
        return 1;
      }
      return 0;
    };

  private:
      uint32_t tmr;
      uint32_t prd;
};