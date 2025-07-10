#ifndef SERVO_PWM_H
#define SERVO_PWM_H

#include <stdint.h> 
#include "pico/stdlib.h" 

#define PINO_SERVO 8             
#define PWM_FREQ 50              
#define PWM_WRAP 19999           

#define SERVO_PULSE_MIN_US 500   // Pulso mínimo em microssegundos (0 graus)
#define SERVO_PULSE_CENTER_US 1500 // Pulso central em microssegundos (90 graus)
#define SERVO_PULSE_MAX_US 2500  // Pulso máximo em microssegundos (180 graus)

void servo_init_pwm(void);
void servo_set_angle(float angle_degrees);

#endif