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
#include <patterns.h>


extern neopixel_strip_t strip[];
extern uint8_t leds_per_strip[];

/**
 * Main program
 */
int main(void)
{
    init_gpio();
    init_ledstrips();
    init_timer();
    init_patterns();

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
             for (s=0; s<LEDSTRIP_COUNT; s++)
                 for (led=0; led<leds_per_strip[s]; led++)
                    neopixel_set_color(&strip[s], led, value, value, value);

            // show
            for (s=0; s<LEDSTRIP_COUNT; s++)
                neopixel_show(&strip[s]);
            // wait
            nrf_delay_ms(3);
        }

        // brightness down
        for (value=60; value>0; value--)
        {

            // set all LEDs
             for (s=0; s<LEDSTRIP_COUNT; s++)
                 for (led=0; led<leds_per_strip[s]; led++)
                    neopixel_set_color(&strip[s], led, value, value, value);

            // show
            for (s=0; s<LEDSTRIP_COUNT; s++)
                neopixel_show(&strip[s]);
            // wait
            nrf_delay_ms(3);
        }
    }
}
