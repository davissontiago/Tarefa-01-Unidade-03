#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"

#include "display.h"
#include "servo.h"
#include "bh1750.h"
#include "leds.h"

// Funções de Lógica Principal
void acender_led_por_lux(float lux) {
    char message[50]; 

    if (lux <= 100) {
        led_low();
        snprintf(message, sizeof(message), "Baixo: %.1f lx", lux);
        servo_send_pulso(1000);
    } else if (lux <= 300) {
        led_mid();
        snprintf(message, sizeof(message), "Medio: %.1f lx", lux);
        servo_send_pulso(1500); 
    } else if (lux <= 700) {
        led_high();
        snprintf(message, sizeof(message), "Alto: %.1f lx", lux);
        servo_send_pulso(2000);
    } else {
        led_very_high();
        snprintf(message, sizeof(message), "High: %.1f lx", lux);
        servo_send_pulso(4000); 
    }
    display_clear();
    display_message(message, 3);
    display_update();
}

// --- Função Principal ---
int main() {
    stdio_init_all(); 

    // Configuração de Periféricos
    leds_setup();
    servo_setup();

    bh1750_init(); 
    display_init(); 

    // Loop Principal
    while (true) {
        float lux = bh1750_read_lux(); 
        if (lux >= 0) { 
            acender_led_por_lux(lux); 
        } else {
            display_message("Erro BH1750", 3);
        }
        sleep_ms(100);
    }
    return 0;
}