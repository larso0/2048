#include "lcg_random.h"

uint32_t Random(uint32_t prev)
{
    //https://en.wikipedia.org/wiki/Lehmer_random_number_generator#Parameters_in_common_use
    return ((uint64_t)prev * 279470273UL) % 4294967291UL;
}
