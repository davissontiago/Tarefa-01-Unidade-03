#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h> 

#define OLED_I2C_PORT i2c1
#define OLED_SDA_PIN 14
#define OLED_SCL_PIN 15
#define OLED_I2C_BAUDRATE 400000

void display_init(void);
void display_clear(void);
void display_message(const char *message, int line);
void display_update(void);

#endif