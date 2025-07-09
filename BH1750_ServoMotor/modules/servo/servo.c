#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "servo.h"

#define PINO_SERVO 8
#define PERIODO_SERVO 20

void configurar_servo() {
    gpio_init(PINO_SERVO);
    gpio_set_dir(PINO_SERVO, GPIO_OUT);
}

void enviar_pulso_servo(uint duty_us) {
    gpio_put(PINO_SERVO, 1);
    sleep_us(duty_us);
    gpio_put(PINO_SERVO, 0);
    sleep_ms(PERIODO_SERVO - (duty_us / 1000));
}


