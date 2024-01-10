class simpleTimer 
{
  public:

    simpleTimer () {};

    simpleTimer (uint32_t nprd) 
    {
      start(nprd);
    };

    void start(uint32_t nprd) 
    {
      prd = nprd;
      start_int();
    };

    void start_int() 
    {
      tmr = millis();
      if (!tmr) tmr = 1;
    };

    void stop() 
    {
      tmr = 0;
    };

    bool ready() 
    {
      if (tmr && millis() - tmr >= prd) 
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