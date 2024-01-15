#pragma once
#include <cstdint>

uint32_t millis(void);
void millisFlush(void);

class timeKeeper
{
    public:
        uint64_t getMillis();
    private:
        uint64_t millis_int = 0ULL;
};