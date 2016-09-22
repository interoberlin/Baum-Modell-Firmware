
#include "tree.h"

// LED strips
#define count_strips 1
neopixel_strip_t strip[count_strips];
const uint8_t leds_per_strip = 10;
const uint8_t strip_at_pin[] = {0};

// to save computing time, update only changed strips 
volatile bool strip_changed[count_strips];

/**
 * This function initializes the used LED strips
 */
void tree_init()
{
    for (int i=0; i < count_strips; i++)
    {
        neopixel_init(&strip[i], strip_at_pin[i], leds_per_strip);
        neopixel_clear(&strip[i]);
    }
}

/**
 * Returns tree strip with specified index
 */
neopixel_strip_t* tree_get_strip(uint8_t index)
{
    return &strip[index];
}

/**
 * This function configures the timer for animations
 */
void tree_setup_timer(void)
{
    // Set the timer to Counter Mode
    NRF_TIMER2->MODE = TIMER_MODE_MODE_Timer;
    // clear the task first to be usable for later
    NRF_TIMER2->TASKS_CLEAR = 1;
    // No shortcuts
    NRF_TIMER2->SHORTS = 0;
    // Set prescaler: 0-9. Higher number gives slower timer. 0 gives 16MHz timer.
    NRF_TIMER2->PRESCALER = 6;
    // Set timer bit resolution
    NRF_TIMER2->BITMODE = TIMER_BITMODE_BITMODE_16Bit;
    // Set timer compare values
    NRF_TIMER2->CC[0] = 1;
    NRF_TIMER2->CC[1] = 32768;

    // Enable interrupt on Timer 2, both for CC[0] and CC[1] compare match events
    NRF_TIMER2->INTENSET =
            (TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos)
          | (TIMER_INTENSET_COMPARE1_Enabled << TIMER_INTENSET_COMPARE1_Pos);
    NVIC_EnableIRQ(TIMER2_IRQn);

    // Start TIMER2
    NRF_TIMER2->TASKS_START = 1;
}

/**
 * Interrupts Service Routine (ISR) for timer 2
 *
 */
/*
void TIMER2_IRQHandler()
{
    if (NRF_TIMER2->EVENTS_COMPARE[0] && (NRF_TIMER2->INTENSET & TIMER_INTENSET_COMPARE0_Msk))
    {
        NRF_TIMER2->EVENTS_COMPARE[0] = 0;

        // update all the strips
        // if they have changed
        for (int i=0; i<8; i++)
        {
            if (strip_changed[i])
            {
                neopixel_show(&strip[i]);
                strip_changed[i] = false;
            }
        }
    }

    else if (NRF_TIMER2->EVENTS_COMPARE[1] && (NRF_TIMER2->INTENSET & TIMER_INTENSET_COMPARE1_Msk))
    {
        NRF_TIMER2->EVENTS_COMPARE[1] = 0;

        // execute pattern calculation
        //proceed_with_pattern();
    }
}
*/