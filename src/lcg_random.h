#ifndef _2048_LCG_RANDOM_H
#define _2048_LCG_RANDOM_H
#include <stdint.h>

/*
 * A linear congruential generator.
 * Use the previous random number as input.
 */
uint32_t Random(uint32_t prev);

#endif
