/**
 * Baumhaus tree firmware
 */

#include <stdbool.h>
#include <stdint.h>

#include <nrf51.h>
#include <nrf_delay.h>
#include <nrf_gpio.h>

#include <led.h>
//#include <clock.h>

#include <pinout.h>
#include <board.h>

// LED strips
#define NUM_STRIPS      4
#define LEDS_PER_STRIP  200
#define TOTAL_NUM_LEDS  (NUM_STRIPS*LEDS_PER_STRIP)
neopixel_strip_t strip[NUM_STRIPS];
const uint8_t strip_at_pin[NUM_STRIPS]   = {PIN_LED_DATA1, PIN_LED_DATA2, PIN_LED_DATA3, PIN_LED_DATA4};
const uint8_t leds_per_strip[NUM_STRIPS] = {LEDS_PER_STRIP, LEDS_PER_STRIP, LEDS_PER_STRIP, LEDS_PER_STRIP};
volatile bool strip_changed[NUM_STRIPS]  = {false, false, false, false};

// workaround:
// static memory allocation
// because malloc is (currently) not working
uint8_t led_memory[NUM_STRIPS*LEDS_PER_STRIP*3];

/**
 * @brief Initializes all LED strips
 */
void init_ledstrips()
{
    uint8_t *memptr = led_memory;
    for (int strip_num=0; strip_num<NUM_STRIPS; strip_num++)
    {
        strip[strip_num].leds = memptr;
        memptr = memptr + LEDS_PER_STRIP*3;
        neopixel_init(&strip[strip_num], strip_at_pin[strip_num], leds_per_strip[strip_num]);
        // unnecessary, filled with zeroes by startup script anyways
        //neopixel_clear(&strip[strip_num]);
    }
}

/**
 * @brief Calculates new intensity values for all LEDs, i.e. a new "frame"
 */
void calculate_new_led_values()
{
    uint8_t current_strip = 0;

    for (uint32_t led=0; led<TOTAL_NUM_LEDS; led++)
    {
        uint8_t warmwhite = 0, coldwhite = 0, amber = 0;
/*
        neopixel_get_color(&strip[current_strip], led, &warmwhite, &coldwhite, &amber);
        warmwhite = ((uint32_t) warmwhite + 1) % 256;
        coldwhite = ((uint32_t) coldwhite + 1) % 256;
        amber = ((uint32_t) amber + 1) % 256;
*/
        neopixel_set_color(&strip[current_strip], led, warmwhite, coldwhite, amber);
    }
    strip_changed[current_strip] = true;
}

/**
 * @brief Configures the FPS timer
 */
void setup_fps_timer(void)
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


/**
 * @brief FPS Timer ISR
 */
void TIMER2_Handler()
{
    // COMPARE0 event occured ?
    if (NRF_TIMER2->EVENTS_COMPARE[0] && (NRF_TIMER2->INTENSET & TIMER_INTENSET_COMPARE0_Msk))
    {
        // clear event
        NRF_TIMER2->EVENTS_COMPARE[0] = 0;

        //nrf_gpio_pin_set(NRFDUINO_PIN_LED);

        // update all the strips
        // if they have changed
        for (int i=0; i<NUM_STRIPS; i++)
        {
            if (strip_changed[i])
            {
                neopixel_show(&strip[i]);
                strip_changed[i] = false;
            }
        }

        calculate_new_led_values();

        //nrf_gpio_pin_clear(NRFDUINO_PIN_LED);
    }
}


/**
 * Main program
 */
int main(void)
{
    init_gpio();
    init_ledstrips();

    powersupply_enable();

/*
works on nRFduino...

    // all on
    while (true)
    {
        uint8_t value = 30;
        for (uint8_t led=0; led<TOTAL_NUM_LEDS; led++)
            neopixel_set_color(&strip[0], led, value, value, value);
        neopixel_show(&strip[0]);
        nrf_delay_ms(2000);

        value = 0;
        for (uint8_t led=0; led<TOTAL_NUM_LEDS; led++)
            neopixel_set_color(&strip[0], led, value, value, value);
        neopixel_show(&strip[0]);
        nrf_delay_ms(2000);
    }
*/

    // infinite loop
     while(true)
     {
        uint8_t value, led, s;

        // up
        for (value=0; value<60; value++)
        {
            // set all LEDs
             for (led=0; led<LEDS_PER_STRIP; led++)
                 for (s=0; s<NUM_STRIPS; s++)
                    neopixel_set_color(&strip[s], led, value, value, value);

            // show
            for (s=0; s<NUM_STRIPS; s++)
                neopixel_show(&strip[s]);
            // wait
            nrf_delay_ms(3);
        }

        // down
        for (value=60; value>0; value--)
        {

            // set all LEDs
             for (led=0; led<LEDS_PER_STRIP; led++)
                 for (s=0; s<NUM_STRIPS; s++)
                    neopixel_set_color(&strip[s], led, value, value, value);

            // show
            for (s=0; s<NUM_STRIPS; s++)
                neopixel_show(&strip[s]);
            // wait
            nrf_delay_ms(3);
        }
     }

    //setup_fps_timer();
}
