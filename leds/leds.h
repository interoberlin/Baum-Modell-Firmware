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

#define LED_COLUMN_LEFT     0
#define LED_COLUMN_RIGHT    1

/**
 * @brief Initializes all LED strips
 */
void init_ledstrips();

/**
 * @brief Set the value of a specific LED
 */
void set_led(uint8_t, uint16_t, uint8_t, uint8_t, uint8_t);

/**
 * @brief Output LED values from RAM to LEDs
 */
void update_leds();

#endif // LEDS_H
