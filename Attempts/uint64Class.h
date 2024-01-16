#pragma once
#include <cstdint>

volatile class int64_manual
{
    public:
        int64_manual();
        int64_manual(const int64_manual&);
        void operator++ (int);
        void inc(void);
        bool operator>= (const int64_manual&);
        int64_manual operator- (const int64_manual&);
        bool isNULL(void);
        void set0(void);
        void set1(void);
        void setHi(uint32_t);
        void setLo(uint32_t);
        uint32_t getHi(void);
        uint32_t getLo(void);
    private: 
        uint32_t hi, lo;
};


//BiggerInt add( const BiggerInt *lhs, const BiggerInt *rhs ) {
//    BiggerInt ret;
//
//    /* Ideally, you'd want a better way to check for overflow conditions */
//    if ( rhs->high < INT_MAX - lhs->high ) {
//        /* With a variable-length (a real) BigInt, you'd allocate some more room here */
//    }
//
//    ret.high = lhs->high + rhs->high;
//
//    if ( rhs->low < INT_MAX - lhs->low ) {
//        /* No overflow */
//        ret.low = lhs->low + rhs->low;
//    }
//    else {
//        /* Overflow */
//        ret.high += 1;
//        ret.low = lhs->low - ( INT_MAX - rhs->low ); /* Right? */
//    }
//
//    return ret;
//}