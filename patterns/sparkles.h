/**
 * This library implements a sparkle effect for addressable LEDs.
 *
 * A specified number of LEDs is initialized,
 * the number and speed of sparkles is configured
 * and e.g. a timer ISR can then facilitate incremental
 * update of LED values.
 *
 * A function rand() must be implemented elsewhere,
 * which returns a random integer between [0;255].
 */

#ifndef SPARKLES_H

extern uint8_t rand();

typedef struct
{
    uint8_t color[3];
} rgb_t;

typedef struct
{
    // by how many update steps to delay the start of this sparkle
    // in order to make them look randomly appearing
    uint32_t steps_delayed;

    // the index of this sparkling LED in the LED memory referenced in sparkle_effect_t
    uint8_t led_index;

    // whether this sparkle currently increases or decreases
    bool increasing;

} sparkle_t;

typedef struct
{
    // pointer to LED value memory
    rgb_t *led_memory;

    // number of LEDs in total
    uint8_t led_count;

    // pointer to the memory region, where individual sparkle configurations are stored
    sparkle_t *sparkle_config;

    // number of simultaneous sparkles
    uint8_t sparkle_count;

    // by which amount to change an LED value per step
    uint8_t increment;

} sparkle_effect_t;


/**
 * Initialize all sparkles with random startup values
 */
void init_sparkles(sparkle_effect_t*);

/**
 * Go through all configured sparkles and apply them to the LED memory,
 * restart them with random values when finished
 */
void update_sparkles(sparkle_effect_t*);

#endif // SPARKLES
