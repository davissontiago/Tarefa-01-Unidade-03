#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "inc/ssd1306.h"

// --- Definições de Hardware ---

// Pinos dos LEDs RGB
#define LED_GREEN 11
#define LED_BLUE 12
#define LED_RED 13

// Pino e período do Servo Motor
#define PINO_SERVO 8
#define PERIODO_SERVO 20 // Período total do PWM em ms (20ms para a maioria dos servos)

// Configuração I2C para o sensor BH1750 (I2C0)
#define BH1750_I2C_PORT               i2c0
#define BH1750_SDA_PIN                0
#define BH1750_SCL_PIN                1
#define BH1750_ADDR                   0x23
#define BH1750_CONTINUOUS_HIGH_RES_MODE 0x10

// Configuração I2C para o display OLED (I2C1)
#define OLED_I2C_PORT                 i2c1
#define OLED_SDA_PIN                  14
#define OLED_SCL_PIN                  15

// --- Configurações de PWM (para LEDs, se aplicável) ---
const uint16_t PWM_PERIODO_LEDS = 200; // Ciclo completo do PWM para LEDs (de 0 a 199)
const float PWM_CLOCK_DIVISOR = 16.0f; // Divisor do clock do PWM para LEDs

// --- Variáveis Globais para o OLED ---
struct render_area frame_area;
uint8_t ssd_buffer[ssd1306_buffer_length];

// --- Protótipos de Funções ---

// Funções de Configuração e Inicialização
void configurar_leds();
void configurar_servo();
void init_bh1750();
void init_oled();

// Funções de Leitura
float read_bh1750_lux();

// Funções de Controle do OLED
void display_message(const char *message);

// Funções de Controle dos LEDs
void acender_led_baixo();
void acender_led_medio();
void acender_led_alto();
void acender_led_muito_alto();

// Função de Controle do Servo
void enviar_pulso(uint duty_us);

// Funções de Lógica Principal
void acender_led_por_lux(float lux);

// --- Implementação das Funções ---

// Funções de Configuração e Inicialização
void configurar_leds() {
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_init(LED_BLUE);
    gpio_set_dir(LED_BLUE, GPIO_OUT);
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    // As configurações de PWM para LEDs (setup_pwm_led) não são usadas se o controle for GPIO_OUT direto.
    // Se precisar de PWM para os LEDs, descomente e ajuste:
    // setup_pwm_led(LED_RED);
    // setup_pwm_led(LED_BLUE);
    // setup_pwm_led(LED_GREEN);
}

void configurar_servo() {
    gpio_init(PINO_SERVO);
    gpio_set_dir(PINO_SERVO, GPIO_OUT);
}

void init_bh1750() {
    // Inicializa I2C0 para o sensor BH1750
    i2c_init(BH1750_I2C_PORT, 100 * 1000); // 100 kHz
    gpio_set_function(BH1750_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(BH1750_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(BH1750_SDA_PIN);
    gpio_pull_up(BH1750_SCL_PIN);

    uint8_t buf[1] = {BH1750_CONTINUOUS_HIGH_RES_MODE};
    i2c_write_blocking(BH1750_I2C_PORT, BH1750_ADDR, buf, 1, false);
}

void init_oled() {
    // Inicializa I2C1 para o display OLED
    i2c_init(OLED_I2C_PORT, 400 * 1000); // 400 kHz (mais comum para OLED)
    gpio_set_function(OLED_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(OLED_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(OLED_SDA_PIN);
    gpio_pull_up(OLED_SCL_PIN);

    // Inicializa o hardware do display
    ssd1306_init();
    // Define a área de renderização para preencher todo o display
    frame_area.start_column = 0;
    frame_area.end_column = ssd1306_width - 1;
    frame_area.start_page = 0;
    frame_area.end_page = ssd1306_n_pages - 1;
    calculate_render_area_buffer_length(&frame_area); // Calcula o tamanho do buffer necessário
    memset(ssd_buffer, 0, ssd1306_buffer_length);     // Limpa o buffer para uma tela preta inicial

    // Pequeno atraso para garantir que o OLED inicialize completamente
    sleep_ms(100);
}

// Funções de Leitura
float read_bh1750_lux() {
    uint8_t data[2];
    int bytes_read = i2c_read_blocking(BH1750_I2C_PORT, BH1750_ADDR, data, 2, false);
    if (bytes_read != 2) {
        printf("Erro ao ler o sensor BH1750\n");
        return -1.0f;
    }
    uint16_t raw_value = (data[0] << 8) | data[1];
    return raw_value / 1.2f; // Conversão para lux
}

// Funções de Controle do OLED
void display_message(const char *message) {
    memset(ssd_buffer, 0, ssd1306_buffer_length); // Limpa todo o buffer do display
    ssd1306_draw_string(ssd_buffer, 5, 0, message); // Desenha a string no buffer
    render_on_display(ssd_buffer, &frame_area);     // Envia o buffer para o display
}

// Funções de Controle dos LEDs
void acender_led_baixo() {
    gpio_put(LED_RED, 1);
    gpio_put(LED_BLUE, 0);
    gpio_put(LED_GREEN, 0);
}

void acender_led_medio() {
    gpio_put(LED_RED, 1);
    gpio_put(LED_BLUE, 0);
    gpio_put(LED_GREEN, 1);
}

void acender_led_alto() {
    gpio_put(LED_RED, 0);
    gpio_put(LED_BLUE, 0);
    gpio_put(LED_GREEN, 1);
}

void acender_led_muito_alto() {
    gpio_put(LED_RED, 1);
    gpio_put(LED_BLUE, 1);
    gpio_put(LED_GREEN, 1);
}

// Função de Controle do Servo
void enviar_pulso(uint duty_us) {
    gpio_put(PINO_SERVO, 1);
    sleep_us(duty_us);
    gpio_put(PINO_SERVO, 0);
    // Garante que o período total seja 20ms
    sleep_ms(PERIODO_SERVO - (duty_us / 1000));
}

// Funções de Lógica Principal
void acender_led_por_lux(float lux) {
    char message[50]; // Buffer para a mensagem do display

    if (lux <= 100) {
        acender_led_baixo();
        snprintf(message, sizeof(message), "Baixo: %.1f lx", lux);
        enviar_pulso(1000); // Exemplo: posição para baixa luz
    } else if (lux <= 300) {
        acender_led_medio();
        snprintf(message, sizeof(message), "Medio: %.1f lx", lux);
        enviar_pulso(1500); // Exemplo: posição média
    } else if (lux <= 700) {
        acender_led_alto();
        snprintf(message, sizeof(message), "Alto: %.1f lx", lux);
        enviar_pulso(2000); // Exemplo: posição para alta luz
    } else {
        acender_led_muito_alto();
        snprintf(message, sizeof(message), "Altissimo: %.1f lx", lux);
        enviar_pulso(4000); // Exemplo: mantém posição para luz muito alta
    }
    display_message(message);
}

// --- Função Principal ---
int main() {
    stdio_init_all(); // Inicializa comunicação serial para debug

    // Configuração de Periféricos
    configurar_leds();
    configurar_servo();
    init_bh1750(); // Inicializa o sensor de luminosidade
    init_oled();  // Inicializa o display OLED

    // Mensagem de Início
    display_message("Modo Automatico");
    sleep_ms(2000); // Exibe a mensagem por 2 segundos

    // Loop Principal
    while (true) {
        float lux = read_bh1750_lux(); // Lê a luminosidade
        if (lux >= 0) { // Se a leitura for válida
            acender_led_por_lux(lux); // Acende o LED correspondente e controla o servo
        } else {
            display_message("Erro BH1750"); // Exibe erro no display se a leitura falhar
        }

        sleep_ms(100); // Pequeno atraso no loop principal para não sobrecarregar
    }

    return 0; // O programa nunca deve chegar aqui em sistemas embarcados típicos
}