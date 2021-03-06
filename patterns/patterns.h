
#ifndef PATTERNS_H
#define PATTERNS_H

#include <stdint.h>
#include <stdbool.h>

#include "sparkles.h"

#define SPARKLE_COUNT   1

/**
 * @brief Initialize all available patterns
 */
void init_patterns();

/**
 * @brief Calculates new intensity values for all LEDs, \
 *        i.e. calculate the next "pattern frame" \
 *        for the currently selected pattern(s)
 */
void update_patterns();

#endif // PATTERNS_H
