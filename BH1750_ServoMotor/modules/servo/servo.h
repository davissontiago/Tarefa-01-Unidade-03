#ifndef SERVO_H
#define SERVO_H

#include <stdint.h> 

void servo_setup();
void servo_send_pulso(uint duty_us);

#endif



