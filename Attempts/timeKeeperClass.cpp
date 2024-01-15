#include "include/timeKeeperClass.h"

uint64_t timeKeeper::getMillis()
{
    this->millis_int += (uint64_t)millis();
    millisFlush();
    return millis_int;
}