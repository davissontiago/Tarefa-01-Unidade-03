#ifndef SERVO_H
#define SERVO_H

#include <stdint.h> 

#define PINO_SERVO 8
#define PERIODO_SERVO 20

void servo_setup();
void servo_send_pulso(uint duty_us);

#endif