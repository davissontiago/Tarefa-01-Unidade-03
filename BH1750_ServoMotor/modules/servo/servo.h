#ifndef SERVO_H
#define SERVO_H

#include <stdint.h> 

void configurar_servo();
void enviar_pulso_servo(uint duty_us);

#endif



