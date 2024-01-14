#include "include/uint64Class.h"

int64_manual::int64_manual()
{
    this->set0();
}

void int64_manual::setHi(uint32_t newHi)
{
    this->hi = newHi;
}

void int64_manual::setLo(uint32_t newLo)
{
    this->lo = newLo;
}

uint32_t int64_manual::getHi(void)
{
    return this->hi;
}

uint32_t int64_manual::getLo(void)
{
    return this->lo;
}

void int64_manual::operator++ (int)
{
    this->inc();
}

void int64_manual::inc(void)
{
    if(lo != __UINT32_MAX__)
    {
        this->lo++;
    }
    else
    {
        this->lo = 0UL;
        this->hi++; 
    }
}

void int64_manual::set0(void)
{
    this->hi = 0UL;
    this->lo = 0UL;
}

void int64_manual::set1(void)
{
    this->hi = 0UL;
    this->lo = 1UL;
}

bool int64_manual::isNULL(void)
{
    return (this->hi == 0UL) && (this->lo == 0UL);
}

bool int64_manual::operator>= (const int64_manual& rightOP)
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

int64_manual int64_manual::operator- (const int64_manual& rightOP)
{
    int64_manual result;

    if(this->lo < rightOP.lo)
    {
        result.setHi(this->hi - 1UL - rightOP.hi);
        result.setLo(__INT32_MAX__ - rightOP.lo + this->lo);
    }
    else
    {
        result.setLo(this->lo - rightOP.lo);
        result.setHi(this->hi - rightOP.hi);
    }

    return result;
}