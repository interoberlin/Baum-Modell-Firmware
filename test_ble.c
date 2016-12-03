
#include <stdbool.h>

#include <nrf_gpio.h>

#include "board.h"
#include "bluetooth.h"

void on_ble_connected()
{
    // TODO
}

void on_ble_disconnected()
{
    // TODO
}

void on_characteristic_written(uint16_t uuid, uint8_t* data, uint8_t length)
{
    // TODO
}

void main()
{
    ble_init();

    while (true)
    {
        asm("wfi");
    }
}
