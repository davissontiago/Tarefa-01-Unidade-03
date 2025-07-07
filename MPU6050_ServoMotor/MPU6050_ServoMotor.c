#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#include "inc/ssd1306.h"

#define MPU6050_ADDR 0x68
#define MPU6050_PWR_MGMT_1 0x6B
#define MPU6050_ACCEL_XOUT_H 0x3B
#define ACCEL_SCALE_FACTOR 16384.0f

#define MPU6050_I2C_PORT i2c0
#define MPU6050_SDA_PIN 0
#define MPU6050_SCL_PIN 1
#define MPU6050_I2C_BAUDRATE 100000

#define OLED_I2C_PORT i2c1
#define OLED_SDA_PIN 14
#define OLED_SCL_PIN 15
#define OLED_I2C_BAUDRATE 400000

#define PINO_SERVO 8
#define PWM_FREQ 50
#define PWM_WRAP 19999

#define SERVO_PULSE_MIN_US 500
#define SERVO_PULSE_CENTER_US 1500
#define SERVO_PULSE_MAX_US 2500

struct render_area frame_area;
uint8_t ssd_buffer[ssd1306_buffer_length];

#define ANGULO_ALERTA_GRAUS 30.0f

void mpu6050_init();
void mpu6050_read_raw_data(int16_t accel[3], int16_t gyro[3]);
void calculate_angles(int16_t accel[3], float *pitch, float *roll);

void init_servo_pwm();
uint32_t angle_to_duty_cycles(float angle_degrees);
void set_servo_angle(float angle_degrees);

void init_oled();
void clear_oled_display();
void display_message_oled(const char *message, int line);
void display_angles_oled(float pitch, float roll, const char *alerta_str);

void mpu6050_init() {
    i2c_init(MPU6050_I2C_PORT, MPU6050_I2C_BAUDRATE);
    gpio_set_function(MPU6050_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(MPU6050_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(MPU6050_SDA_PIN);
    gpio_pull_up(MPU6050_SCL_PIN);

    printf("I2C0 para MPU6050 configurado.\n");
    sleep_ms(100);

    uint8_t buf[2];
    buf[0] = MPU6050_PWR_MGMT_1;
    buf[1] = 0x00;
    
    int ret = i2c_write_blocking(MPU6050_I2C_PORT, MPU6050_ADDR, buf, 2, false);
    if (ret == PICO_ERROR_GENERIC) {
        printf("Erro ao acordar MPU6050! Verifique conexoes e endereco I2C.\n");
    } else {
        printf("MPU6050 acordado e inicializado com sucesso.\n");
    }
    sleep_ms(100);
}

void mpu6050_read_raw_data(int16_t accel[3], int16_t gyro[3]) {
    uint8_t buffer[14];

    uint8_t reg_addr = MPU6050_ACCEL_XOUT_H;
    int ret = i2c_write_blocking(MPU6050_I2C_PORT, MPU6050_ADDR, &reg_addr, 1, true);
    if (ret == PICO_ERROR_GENERIC) {
        printf("Erro ao solicitar leitura de dados do MPU6050.\n");
        memset(accel, 0, sizeof(int16_t)*3);
        memset(gyro, 0, sizeof(int16_t)*3);
        return;
    }
    
    ret = i2c_read_blocking(MPU6050_I2C_PORT, MPU6050_ADDR, buffer, 14, false);
    if (ret == PICO_ERROR_GENERIC) {
        printf("Erro ao ler dados do MPU6050.\n");
        memset(accel, 0, sizeof(int16_t)*3);
        memset(gyro, 0, sizeof(int16_t)*3);
        return;
    }

    accel[0] = (int16_t)((buffer[0] << 8) | buffer[1]);
    accel[1] = (int16_t)((buffer[2] << 8) | buffer[3]);
    accel[2] = (int16_t)((buffer[4] << 8) | buffer[5]);

    gyro[0] = (int16_t)((buffer[8] << 8) | buffer[9]);
    gyro[1] = (int16_t)((buffer[10] << 8) | buffer[11]);
    gyro[2] = (int16_t)((buffer[12] << 8) | buffer[13]);
}

void calculate_angles(int16_t accel[3], float *pitch, float *roll) {
    float accel_x = (float)accel[0] / ACCEL_SCALE_FACTOR;
    float accel_y = (float)accel[1] / ACCEL_SCALE_FACTOR;
    float accel_z = (float)accel[2] / ACCEL_SCALE_FACTOR;

    *pitch = atan2f(accel_x, sqrtf(accel_y * accel_y + accel_z * accel_z)) * 180.0f / M_PI;
    *roll = atan2f(accel_y, sqrtf(accel_x * accel_x + accel_z * accel_z)) * 180.0f / M_PI;
}

void init_servo_pwm() {
    gpio_set_function(PINO_SERVO, GPIO_FUNC_PWM);

    uint slice_num = pwm_gpio_to_slice_num(PINO_SERVO);
    uint chan_num = pwm_gpio_to_channel(PINO_SERVO);

    pwm_set_wrap(slice_num, PWM_WRAP);
    pwm_set_clkdiv(slice_num, 125.0f);

    pwm_set_chan_level(slice_num, chan_num, 0);
    pwm_set_enabled(slice_num, true);
    printf("Servo PWM configurado no pino GP%d.\n", PINO_SERVO);
}

uint32_t angle_to_duty_cycles(float angle_degrees) {
    uint32_t duty_us = (uint32_t)(SERVO_PULSE_MIN_US +
                                  (angle_degrees / 180.0f) * (SERVO_PULSE_MAX_US - SERVO_PULSE_MIN_US));
    
    if (duty_us < SERVO_PULSE_MIN_US) duty_us = SERVO_PULSE_MIN_US;
    if (duty_us > SERVO_PULSE_MAX_US) duty_us = SERVO_PULSE_MAX_US;
    
    return duty_us;
}

void set_servo_angle(float angle_degrees) {
    uint slice_num = pwm_gpio_to_slice_num(PINO_SERVO);
    uint chan_num = pwm_gpio_to_channel(PINO_SERVO);
    
    uint32_t duty_us = angle_to_duty_cycles(angle_degrees);
    
    pwm_set_chan_level(slice_num, chan_num, duty_us);
}

void init_oled() {
    i2c_init(OLED_I2C_PORT, OLED_I2C_BAUDRATE);
    gpio_set_function(OLED_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(OLED_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(OLED_SDA_PIN);
    gpio_pull_up(OLED_SCL_PIN);

    ssd1306_init();
    frame_area.start_column = 0;
    frame_area.end_column = ssd1306_width - 1;
    frame_area.start_page = 0;
    frame_area.end_page = ssd1306_n_pages - 1;
    calculate_render_area_buffer_length(&frame_area);
    memset(ssd_buffer, 0, ssd1306_buffer_length);

    sleep_ms(100);
    printf("Display OLED inicializado no I2C1.\n");
}

void clear_oled_display() {
    memset(ssd_buffer, 0, ssd1306_buffer_length);
}

void display_message_oled(const char *message, int line) {
    ssd1306_draw_string(ssd_buffer, 5, line * 8, (char *)message);
}

void display_angles_oled(float pitch, float roll, const char *alerta_str) {
    char pitch_str[32];
    char roll_str[32];

    snprintf(pitch_str, sizeof(pitch_str), "Pitch: %.1f deg", pitch);
    snprintf(roll_str, sizeof(roll_str), "Roll:  %.1f deg", roll);

    clear_oled_display();

    display_message_oled(pitch_str, 0);
    display_message_oled(roll_str, 2);

    if (alerta_str[0] != '\0') {
        display_message_oled(alerta_str, 4);
    }
    
    render_on_display(ssd_buffer, &frame_area);
}

int main() {
    stdio_init_all();

    printf("Iniciando sistema de monitoramento de inclinacao e controle de servo...\n");

    init_oled();
    init_servo_pwm();
    mpu6050_init();

    clear_oled_display();
    display_message_oled("MPU6050 + Servo", 0);
    display_message_oled("Pronto!", 2);
    render_on_display(ssd_buffer, &frame_area);
    sleep_ms(2000);
    clear_oled_display();

    float pitch = 0.0f;
    float roll = 0.0f;
    int16_t accel_data[3];
    int16_t gyro_data[3];
    char alerta_str[32];

    set_servo_angle(90.0f);
    sleep_ms(500);

    while (true) {
        mpu6050_read_raw_data(accel_data, gyro_data);
        calculate_angles(accel_data, &pitch, &roll);

        printf("Pitch: %.2f deg, Roll: %.2f deg\n", pitch, roll);

        float servo_angle = pitch + 90.0f;
        if (servo_angle < 0.0f) {
            servo_angle = 0.0f;
        }
        if (servo_angle > 180.0f) {
            servo_angle = 180.0f;
        }
        set_servo_angle(servo_angle);

        if (fabs(pitch) > ANGULO_ALERTA_GRAUS || fabs(roll) > ANGULO_ALERTA_GRAUS) {
            snprintf(alerta_str, sizeof(alerta_str), "ALERTA: Inclinado!");
        } else {
            alerta_str[0] = '\0';
        }

        display_angles_oled(pitch, roll, alerta_str);

        sleep_ms(100);
    }

    return 0;
}