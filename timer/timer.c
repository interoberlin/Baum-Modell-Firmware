
#include "timer.h"

void init_timer()
{
    // Set the timer to Counter Mode
    NRF_TIMER2->MODE = TIMER_MODE_MODE_Timer;
    // clear the task first to be usable for later
    NRF_TIMER2->TASKS_CLEAR = 1;
    // No shortcuts
    NRF_TIMER2->SHORTS = 0;
    /*
     * Set timing frequency via prescaler: 0-9
     * Higher numbers result in lower lower frequency
     *
     * f = 16 MHz / (2**prescaler)
     *
     * 0 => 16 MHz
     * 6 => 250 kHz
     */
    NRF_TIMER2->PRESCALER = 2;
    // Set timer bit resolution
    NRF_TIMER2->BITMODE = TIMER_BITMODE_BITMODE_16Bit;
    // Set timer compare values
    NRF_TIMER2->CC[0] = (1 >> 16)-1;

    // Enable interrupt on Timer 2, both for CC[0] and CC[1] compare match events
    NRF_TIMER2->INTENSET =
            (TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos);
          //| (TIMER_INTENSET_COMPARE1_Enabled << TIMER_INTENSET_COMPARE1_Pos);
    NVIC_EnableIRQ(TIMER2_IRQn);

    // Start TIMER2
    NRF_TIMER2->TASKS_START = 1;
}

void TIMER2_Handler()
{
    // COMPARE0 event occured ?
    if (NRF_TIMER2->EVENTS_COMPARE[0] && (NRF_TIMER2->INTENSET & TIMER_INTENSET_COMPARE0_Msk))
    {
        // clear event
        NRF_TIMER2->EVENTS_COMPARE[0] = 0;

        // make sure, only one instance is running at a time
        static volatile bool busy = false;
        if (busy)
            return;
        busy = true;

        // toggle LED for debugging
        #ifdef BOARD_NRFDUINO
        nrf_gpio_pin_toggle(NRFDUINO_PIN_LED);
        #endif

        update_patterns();

        // update all the strips
        // if they have changed
        update_leds();

        busy = false;
    }
}
