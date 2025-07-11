#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "servo.h"

void servo_setup() {
    gpio_init(PINO_SERVO);
    gpio_set_dir(PINO_SERVO, GPIO_OUT);
}

void servo_send_pulso(uint duty_us) {
    gpio_put(PINO_SERVO, 1);
    sleep_us(duty_us);
    gpio_put(PINO_SERVO, 0);
    sleep_ms(PERIODO_SERVO - (duty_us / 1000));
}