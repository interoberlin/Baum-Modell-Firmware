/**
 * Baumhaus tree demo firmware
 *
 * by Thee Vanichangkul, Matthias Bock and Long Pham
 */

#include <stdbool.h>
#include <stdint.h>

#include "nrf51.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"

#include "neopixel.h"

// LEd strips
neopixel_strip_t strip[8];
const uint8_t leds_per_strip = 10;
const uint8_t strip_at_pin[8] = {3, 4, 2, 0, 29, 25, 23, 21};


/**
 * This function initializes the used LED strips
 */
void init_ledstrips()
{
    for (int i=0; i<8; i++)
    {
        neopixel_init(&strip[i], strip_at_pin[i], leds_per_strip);
        neopixel_clear(&strip[i]);
    }
}

/**
 * This function configures the timers we require
 */
void setup_timers(void)
{
    // Set the timer to Counter Mode
    NRF_TIMER2->MODE = TIMER_MODE_MODE_Timer;
    // clear the task first to be usable for later
    NRF_TIMER2->TASKS_CLEAR = 1;
    // Set prescaler: 0-9. Higher number gives slower timer. 0 gives 16MHz timer.
    NRF_TIMER2->PRESCALER = 8;
    // Set timer bit resolution
    NRF_TIMER2->BITMODE = TIMER_BITMODE_BITMODE_16Bit;
    // Set timer compare values
    NRF_TIMER2->CC[0] = 1;
    NRF_TIMER2->CC[1] = 65500;

    // Enable interrupt on Timer 2, both for CC[0] and CC[1] compare match events
    NRF_TIMER2->INTENSET = (TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos) | (TIMER_INTENSET_COMPARE1_Enabled << TIMER_INTENSET_COMPARE1_Pos);
    NVIC_EnableIRQ(TIMER2_IRQn);

    // Start TIMER2
    NRF_TIMER2->TASKS_START = 1;
}

uint8_t r = 0;

/**
 * Interrupts Service Routine (ISR) for the timer
 *
 */
void TIMER2_IRQHandler()
{
    if (NRF_TIMER2->EVENTS_COMPARE[1] && (NRF_TIMER2->INTENSET & TIMER_INTENSET_COMPARE1_Msk))
    {
        // vorherige aus
        for(int i = 5; i<10; i++)
        {
            neopixel_set_color(&strip[r], i, 0, 0, 0);
        }
        neopixel_show(&strip[r]);

        r = (r+1) % 8;

        // naechste an
        for(int i = 5; i<10; i++)
        {
            neopixel_set_color(&strip[r], i, 0, 0, 10);
        }
        neopixel_show(&strip[r]);
    }
}

int main(void)
{
    init_ledstrips();
    setup_timers();

	while(true)
	{
	    asm("wfi"); // sleep: wait for interrupt
	}
}
