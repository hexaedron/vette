#pragma once

// Это от Алексея из чата

class u64{
  private:
    volatile uint32_t lo;
    volatile uint32_t hi;

  public:
    u64(uint32_t lo_value, uint32_t hi_value) : lo(lo_value), hi(hi_value) {};
    u64(uint32_t lo_value) : lo(lo_value), hi(0) {};
    u64(u64 const &value) : lo(value.lo), hi(value.hi) {};
    u64() : lo(0), hi(0) {};

    u64 operator + (const uint32_t& add) const
    {
      const uint32_t lo32_sum = lo + add;
      uint32_t hi32_sum = (uint32_t) (hi + add);
      if (lo32_sum < lo) { // Перенос при сложении возник 
        hi32_sum += 1;
      }
      return u64(lo32_sum, hi32_sum);
    }

    u64 operator + (const u64& add) const
    {
      const uint32_t lo32_sum = lo + add.lo;
      uint32_t hi32_sum = (uint32_t) (hi + add.hi);
      if (lo32_sum < lo) { // Перенос при сложении возник 
        hi32_sum += 1;
      }
      return u64(lo32_sum, hi32_sum);
    }

    u64 operator - (const uint32_t& sub) const
    {
      const uint32_t lo32_sum = lo - sub;
      uint32_t hi32_sum = (uint32_t) (hi - sub);
      if (lo32_sum > lo) { // Перенос при вычитании возник 
        hi32_sum -= 1;
      }
      return u64(lo32_sum, hi32_sum);
    }

    u64 operator - (const u64& sub) const
    {
      const uint32_t lo32_sum = lo - sub.lo;
      uint32_t hi32_sum = (uint32_t) (hi - sub.hi);
      if (lo32_sum > lo) { // Перенос при вычитании возник 
        hi32_sum -= 1;
      }
      return u64(lo32_sum, hi32_sum);
    }

    u64& operator++ (int) {
        const uint32_t lo32_increment = lo + 1;
        if (lo32_increment < lo) { // Перенос при сложении возник 
            hi += 1;
        }

        lo = lo32_increment;
        return *this;
    }

    void Inc(void) {
        const uint32_t lo32_increment = lo + 1;
        if (lo32_increment < lo) { // Перенос при сложении возник 
            hi += 1;
        }

        lo = lo32_increment;
    }

    bool operator >= (const u64& rightOP)
    {
        bool res = (this->hi != rightOP.hi);
        if(res)
        {
            return this->hi > rightOP.hi;
        }
        else
        {
            return this->lo >= rightOP.lo;
        }
    }

    bool operator == (const uint32_t& rightOP)
    {
        return ( (this->hi == 0UL) && (this->lo == rightOP) );
    }

    void operator = (const uint32_t& new_lo) 
    {
      this->lo = new_lo;
      this->hi = 0UL;
    }
};
