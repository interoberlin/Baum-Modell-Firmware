
#include <stdbool.h>

#include <nrf_gpio.h>

#include "bluetooth.h"

#include "board.h"

void on_ble_connected()
{

}

void on_ble_disconnected()
{

}

void on_characteristic_written(uint16_t uuid, uint8_t* data, uint8_t length)
{
    nrf_gpio_pin_toggle(PIN_LED_DATA);
}

void main()
{
    nrf_gpio_cfg_output(PIN_LED_DATA);

    ble_init();

    while (true)
    {
        asm("wfi");
    }
}
