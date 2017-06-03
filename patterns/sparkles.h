/**
 * This library implements a sparkle effect for addressable LEDs.
 */

#ifndef SPARKLES_H
#define SPARKLES_H

#include <random.h>
#include <leds.h>

typedef struct
{
    // the local frame counter
    uint16_t t;

    // index of the frame, at which the sparkle is complete
    uint16_t t_end;

    // index of the frame, at which to begin fading in
    uint16_t t_fade_in;

    // index of the frame, at which to begin fading out
    uint16_t t_fade_out;

    // abstract index of the sparkling LED
    led_index_t led;
} sparkle_t;


/**
 * Initialize the sparkle with random values
 */
void init_sparkle(sparkle_t*);

/**
 * Calculate current sparkle values and update selected LED
 */
void sparkle_update(sparkle_t*);

/**
 * Determine, whether the sparkle is complete or not
 */
bool is_sparkle_finished(sparkle_t*);

#endif // SPARKLES_H
