#ifndef AHT10_H
#define AHT10_H

#include <stdint.h> 

#define AHT10_I2C_PORT i2c0
#define AHT10_SDA_PIN 0
#define AHT10_SCL_PIN 1
#define AHT10_I2C_BAUDRATE 100000

#define AHT10_ADDR 0x38
#define AHT10_CMD_INITIALIZE 0xE1
#define AHT10_CMD_MEASURE 0xAC
#define AHT10_CMD_SOFT_RESET 0xBA
#define AHT10_STATUS_BUSY_MASK 0x80
#define AHT10_STATUS_CAL_MASK 0x08

void aht10_init();
void aht10_reset();
bool aht10_read_data(float *humidity, float *temperature);

#endif