/**
 * Board-related routines
 */

#ifndef BOARD_H
#define BOARD_H

#include <nrf_gpio.h>
#include <pinout.h>

/**
 * Initialize all GPIO pins
 */
#define init_gpio() \
    nrf_gpio_cfg_output(PIN_LED_DATA1); \
    nrf_gpio_cfg_output(PIN_LED_DATA2); \
    nrf_gpio_cfg_output(PIN_LED_DATA3); \
    nrf_gpio_cfg_output(PIN_LED_DATA4); \
    \
    nrf_gpio_cfg_output(PIN_ATX_ON); \
    \
    nrf_gpio_cfg_input(PIN_ATX_OK, NRF_GPIO_PIN_NOPULL); \
    \
    nrf_gpio_cfg_input(PIN_FUSE_OK, NRF_GPIO_PIN_NOPULL); \
    \
    nrf_gpio_cfg_input(PIN_CURRENT_SENSOR, NRF_GPIO_PIN_NOPULL);

#endif // BOARD_H
