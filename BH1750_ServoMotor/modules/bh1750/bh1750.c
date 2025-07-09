#include <stdio.h>
#include <string.h>
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "bh1750.h"

#define BH1750_I2C_PORT i2c0
#define BH1750_SDA_PIN 0
#define BH1750_SCL_PIN 1
#define BH1750_ADDR 0x23
#define BH1750_CONTINUOUS_HIGH_RES_MODE 0x10

void bh1750_init() {
    i2c_init(BH1750_I2C_PORT, 100 * 1000);
    gpio_set_function(BH1750_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(BH1750_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(BH1750_SDA_PIN);
    gpio_pull_up(BH1750_SCL_PIN);

    uint8_t buf[1] = {BH1750_CONTINUOUS_HIGH_RES_MODE};
    i2c_write_blocking(BH1750_I2C_PORT, BH1750_ADDR, buf, 1, false);
}

float bh1750_read_lux() {
    uint8_t data[2];
    int bytes_read = i2c_read_blocking(BH1750_I2C_PORT, BH1750_ADDR, data, 2, false);
    if (bytes_read != 2) {
        printf("Erro ao ler o sensor BH1750\n");
        return -1.0f;
    }
    uint16_t raw_value = (data[0] << 8) | data[1];
    return raw_value / 1.2f; // Conversão para lux
}
