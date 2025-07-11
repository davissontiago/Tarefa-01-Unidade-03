#ifndef BH1750_H
#define BH1750_H

#include <stdint.h> 

#define BH1750_I2C_PORT i2c0
#define BH1750_SDA_PIN 0
#define BH1750_SCL_PIN 1
#define BH1750_ADDR 0x23
#define BH1750_CONTINUOUS_HIGH_RES_MODE 0x10

void bh1750_init();
float bh1750_read_lux();

#endif