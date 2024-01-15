#pragma once

//class simpleTimer 
//{
//  public:
//
//    simpleTimer () {};
//
//    simpleTimer (uint32_t nprd) 
//    {
//      prd.setLo(nprd);
//      start_int();
//    };
//
//    void start_int() 
//    {
//      tmr = millis();
//      if (tmr.isNUL()) tmr.set1();
//    };
//
//    void stop() 
//    {
//      tmr.set0();
//    };
//
//    bool ready() 
//    {
//      int64_manual tmp = (millis() - tmr);
//      if (!tmr.isNUL() && (tmp >= prd)) 
//      {
//        start_int();
//        return 1;
//      }
//      return 0;
//    };
//
//  private:
//      int64_manual tmr;
//      int64_manual prd;
//};

#pragma once

//class simpleTimer 
//{
//  public:
//
//    simpleTimer () {};
//
//    simpleTimer (uint64_t nprd) 
//    {
//      prd = nprd;
//      start_int();
//    };
//
//    void start_int() 
//    {
//      tmr = timeKeep.getMillis();
//      if (tmr == 0ULL) tmr = 1ULL;
//    };
//
//    void stop() 
//    {
//      tmr = 0ULL;
//    };
//
//    bool ready() 
//    {
//      //uint64_t tmp = (timeKeep.getMillis() - tmr);
//      if ((tmr != 0ULL) && ((timeKeep.getMillis() - tmr) >= prd)) 
//      {
//        start_int();
//        return 1;
//      }
//      return 0;
//    };
//
//  private:
//      uint64_t tmr;
//      uint64_t prd;
//};

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