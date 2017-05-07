/**
 * Baumhaus tree firmware
 */

#include <stdbool.h>
#include <stdint.h>

#include <nrf51.h>
#include <nrf_delay.h>

#include <board.h>
#include <leds.h>
#include <timer.h>

extern neopixel_strip_t strip[NUM_STRIPS];

/**
 * Main program
 */
int main(void)
{
    init_gpio();
    init_ledstrips();
    init_timer();

    atx_powersupply_enable();

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

    // infinite loop: all LEDs: brightness up, brightness down
    while(true)
    {
        uint8_t value, led, s;

        // brightness up
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

        // brightness down
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
}
