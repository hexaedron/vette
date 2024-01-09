class simpleTimer 
{
  public:

    simpleTimer () {}

    simpleTimer (uint32_t nprd) 
    {
      start(nprd);
    }

    void start(uint32_t nprd) 
    {
      prd = nprd;
      start();
    }

    void start() 
    {
      tmr = getSecDiv10();
      if (!tmr) tmr = 1;
    }

    void stop() 
    {
      tmr = 0;
    }

    bool ready() 
    {
      if (tmr && getSecDiv10() - tmr >= prd) 
      {
        start();
        return 1;
      }
      return 0;
    }

  private:
    uint32_t tmr = 0, prd = 0;
};