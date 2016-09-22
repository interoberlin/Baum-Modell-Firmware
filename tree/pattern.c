
// variables for rotation pattern
volatile uint32_t r = 0;

inline void pattern_rotation()
{
    // switch previous LED off
    for(int i=0; i<10; i++)
    {
        neopixel_set_color(&strip[r], i, 0, 0, 0);
    }
    strip_changed[r] = true;

    r = (r+1) % 8;

    // switch next LED on
    for(int i=0; i<10; i++)
    {
        neopixel_set_color(&strip[r], i, 0, 0, 10);
    }
    strip_changed[r] = true;
}
