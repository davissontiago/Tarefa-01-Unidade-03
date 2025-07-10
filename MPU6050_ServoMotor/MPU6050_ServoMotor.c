#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#include "mpu6050.h"
#include "servo_pwm.h"
#include "display.h"

#define ANGULO_ALERTA_GRAUS 30.0f

void calculate_angles(int16_t accel[3], float *pitch, float *roll) {
    float accel_x = (float)accel[0] / ACCEL_SCALE_FACTOR;
    float accel_y = (float)accel[1] / ACCEL_SCALE_FACTOR;
    float accel_z = (float)accel[2] / ACCEL_SCALE_FACTOR;

    *pitch = atan2f(accel_x, sqrtf(accel_y * accel_y + accel_z * accel_z)) * 180.0f / M_PI;
    *roll = atan2f(accel_y, sqrtf(accel_x * accel_x + accel_z * accel_z)) * 180.0f / M_PI;
}

int main() {
    stdio_init_all();

    printf("Iniciando sistema de monitoramento de inclinacao e controle de servo...\n");

    display_init();
    servo_init_pwm();
    mpu6050_init();

    display_clear();
    display_message("MPU6050 + Servo", 0);
    display_message("Pronto!", 2);
    display_update();
    sleep_ms(2000);
    display_clear();

    float pitch = 0.0f;
    float roll = 0.0f;
    int16_t accel_data[3];
    int16_t gyro_data[3];
    char alerta_str[32];

    servo_set_angle(90.0f);
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
        servo_set_angle(servo_angle);

        if (fabs(pitch) > ANGULO_ALERTA_GRAUS || fabs(roll) > ANGULO_ALERTA_GRAUS) {
            snprintf(alerta_str, sizeof(alerta_str), "ALERTA: Inclinado!");
        } else {
            alerta_str[0] = '\0';
        }

        display_angles(pitch, roll, alerta_str);

        sleep_ms(100);
    }
    return 0;
}