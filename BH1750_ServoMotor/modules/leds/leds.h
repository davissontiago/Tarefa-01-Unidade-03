#ifndef LEDS_H
#define LEDS_H

#include <stdint.h> 

#define LED_GREEN 11
#define LED_BLUE 12
#define LED_RED 13

void leds_setup();
void led_low();
void led_mid();
void led_high();
void led_very_high();

#endif