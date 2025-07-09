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
        acender_led_baixo();
        snprintf(message, sizeof(message), "Baixo: %.1f lx", lux);
        enviar_pulso_servo(1000);
    } else if (lux <= 300) {
        acender_led_medio();
        snprintf(message, sizeof(message), "Medio: %.1f lx", lux);
        enviar_pulso_servo(1500); 
    } else if (lux <= 700) {
        acender_led_alto();
        snprintf(message, sizeof(message), "Alto: %.1f lx", lux);
        enviar_pulso_servo(2000);
    } else {
        acender_led_muito_alto();
        snprintf(message, sizeof(message), "Altissimo: %.1f lx", lux);
        enviar_pulso_servo(4000); 
    }
    message_display(message, 1);
    update_display();
}

// --- Função Principal ---
int main() {
    stdio_init_all(); 

    // Configuração de Periféricos
    configurar_leds();
    configurar_servo();
    init_bh1750(); 
    init_display(); 

    // Loop Principal
    while (true) {
        float lux = read_bh1750_lux(); 
        if (lux >= 0) { 
            acender_led_por_lux(lux); 
        } else {
            message_display("Erro BH1750", 1);
        }
        sleep_ms(100);
    }
    return 0;
}