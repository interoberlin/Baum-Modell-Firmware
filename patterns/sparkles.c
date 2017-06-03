
#include "sparkles.h"

extern sparkle_t pattern_sparkle[SPARKLE_COUNT];

inline void init_sparkle(sparkle_t *sparkle)
{
    // select a random LED
    bool led_is_occupied;
    uint8_t new_led_index;
    uint8_t new_strip_index;
    do
    {
        new_led_index  = rand() % 10;
        new_strip_index = rand() % 8;

        led_is_occupied = false;
        for (uint8_t i=0; i<SPARKLE_COUNT; i++)
        {
            if (pattern_sparkle[i].led.strip_index == new_strip_index
             && pattern_sparkle[i].led.led_index == new_led_index)
             {
                led_is_occupied = true;
                break;
             }
        }
    } while (led_is_occupied);

    sparkle->led.strip_index = new_strip_index;
    sparkle->led.led_index = new_led_index;

    // reset time
    sparkle->t = 0;

    // set a random fade-in time
    sparkle->t_fade_in = rand() % 10;

    sparkle->duration_fade_in = 5 + (rand() % 10);

    // set fade-out time after fade-in time
    sparkle->t_fade_out = sparkle->t_fade_in + sparkle->duration_fade_in;

    sparkle->duration_fade_out = 5 + (rand() % 15);

    // set end time to when fade-out is complete
    sparkle->t_end = sparkle->t_fade_out + sparkle->duration_fade_out;
}

uint8_t sparkle_color_function(sparkle_t *sparkle)
{
    if (sparkle->t >= sparkle->t_end)
        return 0;

    if (sparkle->t >= sparkle->t_fade_out)
    {
        // fade out
        return 255 - (sparkle->t - sparkle->t_fade_out) * 255/sparkle->duration_fade_out;
    }
    if (sparkle->t >= sparkle->t_fade_in)
    {
        // fade in
       return (sparkle->t - sparkle->t_fade_in) * 255/sparkle->duration_fade_in;
    }
    return 0;
}

inline void sparkle_update(sparkle_t *sparkle)
{
    uint8_t color = sparkle_color_function(sparkle);

    set_led(sparkle->led, color, color, color);
}

inline bool is_sparkle_finished(sparkle_t *sparkle)
{
    return (sparkle->t > sparkle->t_end);
}
