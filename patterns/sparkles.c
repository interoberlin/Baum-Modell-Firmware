
#include "sparkles.h"

inline void init_sparkle(sparkle_t *sparkle)
{
    // reset time
    sparkle->t = 0;

    // select a random LED
/*
    sparkle->led.led_index++;
    if (sparkle->led.led_index > 9)
    {
        sparkle->led.led_index = 0;
        sparkle->led.strip_index++;
        if (sparkle->led.strip_index > 7)
            sparkle->led.strip_index = 0;
    }
*/
    sparkle->led.strip_index = rand() % 8;
    sparkle->led.led_index = rand() % 10;


    // = 0x0001;  //select_random_led();

    // set a random fade-in time
    sparkle->t_fade_in = 3; // random256();

    // set fade-out time after fade-in time
    sparkle->t_fade_out = sparkle->t_fade_in + 7;

    // set end time to when fade-out is complete
    sparkle->t_end = sparkle->t_fade_out + 7;
}

inline uint8_t sparkle_color_function(sparkle_t *sparkle)
{
    if (sparkle->t >= sparkle->t_end)
        return 0;
    if (sparkle->t >= sparkle->t_fade_out)
    {
        // fade out
        return 256 - (sparkle->t - sparkle->t_fade_out) * 256/7;
    }
    if (sparkle->t >= sparkle->t_fade_in)
    {
        // fade in
       return (sparkle->t - sparkle->t_fade_in) * 256/7;
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
