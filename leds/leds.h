/**
 * LED configuration and functions
 * for operating on LED values
 */

#ifndef LEDS_H
#define LEDS_H

#include <stdint.h>
#include <stdbool.h>

#include <board.h>

#include "sk6812.h"


#define NUM_STRIPS      4
#define LEDS_PER_STRIP  150
#define TOTAL_NUM_LEDS  (NUM_STRIPS*LEDS_PER_STRIP)


/**
 * @brief Initializes all LED strips
 */
void init_ledstrips();

/**
 * @brief Calculates new intensity values for all LEDs, \
 *        i.e. calculate the next "pattern frame"
 *
 * @param t The current "time"
 */
void calculate_new_led_values(uint32_t);

/**
 * @brief Output LED values from RAM to LEDs
 */
void update_leds();

#endif // LEDS_H
