#include <stdio.h> 
#include <math.h>  
#include "hardware/gpio.h" 
#include "hardware/pwm.h" 
#include "servo_pwm.h"         

void servo_init_pwm() {
    gpio_set_function(PINO_SERVO, GPIO_FUNC_PWM); 

    uint slice_num = pwm_gpio_to_slice_num(PINO_SERVO); 
    uint chan_num = pwm_gpio_to_channel(PINO_SERVO);   

    pwm_set_wrap(slice_num, PWM_WRAP); // Define o valor máximo do contador (período total do PWM)
    pwm_set_clkdiv(slice_num, 125.0f); // Define o divisor do clock para atingir 50Hz

    pwm_set_chan_level(slice_num, chan_num, 0); // Inicia o servo em uma posição neutra ou mínima (0 duty cycle)
    pwm_set_enabled(slice_num, true); // Habilita o slice PWM
    printf("Servo PWM configurado no pino GP%d.\n", PINO_SERVO);
}

// Função auxiliar (privada do módulo) para converter ângulo em duty cycle
static uint32_t angle_to_duty_cycles(float angle_degrees) {
    // Limita o ângulo para garantir que esteja dentro da faixa esperada
    if (angle_degrees < 0.0f) angle_degrees = 0.0f;
    if (angle_degrees > 180.0f) angle_degrees = 180.0f;

    // Calcula o pulso em microssegundos
    uint32_t duty_us = (uint32_t)(SERVO_PULSE_MIN_US +
                                  (angle_degrees / 180.0f) * (SERVO_PULSE_MAX_US - SERVO_PULSE_MIN_US));
    
    // O valor de duty_us já é o "level" que pwm_set_chan_level espera,
    // pois o clock divider de 125.0f faz com que 1us = 1 ciclo.
    return duty_us;
}

// Define o ângulo do servo
void servo_set_angle(float angle_degrees) {
    uint slice_num = pwm_gpio_to_slice_num(PINO_SERVO);
    uint chan_num = pwm_gpio_to_channel(PINO_SERVO);
    
    uint32_t duty_cycles = angle_to_duty_cycles(angle_degrees);
    
    pwm_set_chan_level(slice_num, chan_num, duty_cycles); // Define o nível do pulso para o servo
}