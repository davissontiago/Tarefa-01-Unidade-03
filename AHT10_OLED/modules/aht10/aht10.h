#ifndef AHT10_H
#define AHT10_H

#include <stdint.h> 

void aht10_init();
void aht10_reset();
bool aht10_read_data(float *humidity, float *temperature);

#endif