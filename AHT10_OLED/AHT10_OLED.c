#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "display.h"
#include "aht10.h"

// --- Função Principal ---
int main()
{
    stdio_init_all();

    printf("Iniciando sistema com AHT10 e OLED...\n");

    init_display();
    clear_display();
    message_display("Iniciando...", 0);
    message_display("AHT10 & OLED", 1);
    
    sleep_ms(2000);
    clear_display();

    aht10_init();

    float humidity, temperature;
    char temp_str[32];
    char hum_str[32];
    char alerta_str[32]; 

    while (true)
    {
        if (aht10_read_data(&humidity, &temperature))
        {
            if (humidity > 70 && temperature < 20)
            {
                snprintf(alerta_str, sizeof(alerta_str), "CRITICO! (U/T)"); 
            }
            else if (humidity > 70)
            {
                snprintf(alerta_str, sizeof(alerta_str), "ALERTA! Umid+");
            }
            else if (temperature < 20)
            {
                snprintf(alerta_str, sizeof(alerta_str), "ALERTA! Temp-");
            }
            else
            {
                alerta_str[0] = '\0';
            }

            printf("Umidade: %.2f %%RH, Temperatura: %.2f C\n", humidity, temperature);

            snprintf(temp_str, sizeof(temp_str), "Temp: %.1f C", temperature);
            snprintf(hum_str, sizeof(hum_str), "Umid: %.1f %%RH", humidity);

            clear_display();

            message_display(temp_str, 0); 
            message_display(hum_str, 2);  

            if (alerta_str[0] != '\0')
            {                                        
                message_display(alerta_str, 4); 
            }

            update_display(); 
        }
        else
        {
            printf("Falha na leitura do AHT10. Tentando resetar...\n");
            clear_display();
            message_display("Erro AHT10!", 0);
            update_display();
            aht10_reset();
            sleep_ms(500);
        }
        sleep_ms(3000);
    }

    return 0;
}