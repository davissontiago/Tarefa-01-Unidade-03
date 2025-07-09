#include <stdio.h>
#include <string.h>
#include "hardware/gpio.h"
#include "leds.h"

#define LED_GREEN 11
#define LED_BLUE 12
#define LED_RED 13

void configurar_leds() {
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_init(LED_BLUE);
    gpio_set_dir(LED_BLUE, GPIO_OUT);
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
}

void acender_led_baixo() {
    gpio_put(LED_RED, 1);
    gpio_put(LED_BLUE, 0);
    gpio_put(LED_GREEN, 0);
}

void acender_led_medio() {
    gpio_put(LED_RED, 1);
    gpio_put(LED_BLUE, 0);
    gpio_put(LED_GREEN, 1);
}

void acender_led_alto() {
    gpio_put(LED_RED, 0);
    gpio_put(LED_BLUE, 0);
    gpio_put(LED_GREEN, 1);
}

void acender_led_muito_alto() {
    gpio_put(LED_RED, 1);
    gpio_put(LED_BLUE, 1);
    gpio_put(LED_GREEN, 1);
}

