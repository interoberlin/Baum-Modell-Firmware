
#ifndef PATTERNS_H
#define PATTERNS_H

#include <stdint.h>
#include <stdbool.h>

/*
typedef
    pattern initializer

typedef struct
{
} pattern_t;
*/

/**
 * @brief Calculates new intensity values for all LEDs, \
 *        i.e. calculate the next "pattern frame"
 *
 * @param t The current "time"
 */
void calculate_new_led_values(uint32_t);

#endif // PATTERNS_H
