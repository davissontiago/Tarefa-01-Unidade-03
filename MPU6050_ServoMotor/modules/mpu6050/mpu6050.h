#ifndef SERVO_MPU6050_H
#define SERVO_MPU6050_H

#include <stdint.h> 
#include "pico/stdlib.h" 

#define MPU6050_ADDR 0x68
#define MPU6050_PWR_MGMT_1 0x6B
#define MPU6050_ACCEL_XOUT_H 0x3B
#define ACCEL_SCALE_FACTOR 16384.0f

#define MPU6050_I2C_PORT i2c0
#define MPU6050_SDA_PIN 0
#define MPU6050_SCL_PIN 1
#define MPU6050_I2C_BAUDRATE 100000

void mpu6050_init();
void mpu6050_read_raw_data(int16_t accel[3], int16_t gyro[3]);

#endif