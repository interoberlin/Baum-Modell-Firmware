
#include "leds.h"

// LED strips
neopixel_strip_t strip[NUM_STRIPS];
const uint8_t strip_at_pin[NUM_STRIPS]   = {PIN_LED_DATA1, PIN_LED_DATA2, PIN_LED_DATA3, PIN_LED_DATA4};
const uint8_t leds_per_strip[NUM_STRIPS] = {LEDS_PER_STRIP, LEDS_PER_STRIP, LEDS_PER_STRIP, LEDS_PER_STRIP};
volatile bool strip_changed[NUM_STRIPS]  = {false, false, false, false};

// static memory allocation for LEDs
// to avoid the use of malloc
uint8_t led_memory[NUM_STRIPS*LEDS_PER_STRIP*3];


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

void calculate_new_led_values(uint32_t t)
{
    uint8_t current_strip = 0;

    for (uint32_t led_index=0; led_index<TOTAL_NUM_LEDS; led_index++)
    {
        uint8_t warmwhite = 0, coldwhite = 0, amber = 0;
/*
        neopixel_get_color(&strip[current_strip], led_index, &warmwhite, &coldwhite, &amber);
        warmwhite = ((uint32_t) warmwhite + 1) % 256;
        coldwhite = ((uint32_t) coldwhite + 1) % 256;
        amber = ((uint32_t) amber + 1) % 256;
*/
        neopixel_set_color(&strip[current_strip], led_index, warmwhite, coldwhite, amber);
    }
    strip_changed[current_strip] = true;
}

void update_leds()
{
    for (uint8_t strip_index=0; strip_index<NUM_STRIPS; strip_index++)
    {
        if (strip_changed[strip_index])
        {
            neopixel_show(&strip[strip_index]);
            strip_changed[strip_index] = false;
        }
    }
}
