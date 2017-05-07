/**
 * TIMER2 is used to compute new LED values
 * and update the LED strips
 */

#ifndef TIMER_H
#define TIMER_H

// The timer will update the LEDs
#include <leds.h>
#include <patterns.h>

// If we're running on nRFduino, blink the LED
#ifdef BOARD_NRFDUINO
#include <nrfduino.h>
#include <nrf_gpio.h>
#endif

/**
 * @brief Sets up the pattern timer
 */
void init_timer();

#endif // TIMER_H
