#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "ws2812.pio.h"  // Biblioteca para controle dos LEDs WS2812
#include "hardware/pio.h"
#include "pico/time.h"

#define LED_R 13
#define LED_G 11
#define LED_B 12
#define BUTTON_A 5
#define BUTTON_B 6
#define MATRIX_LED 7
#define NUM_PIXELS 25  // Matriz 5x5

PIO pio = pio0; 
uint sm = 0; 

volatile int number = 0;

// Buffer de LEDs para armazenar os valores
uint32_t led_buffer[NUM_PIXELS];

void iniciar_leds(){
    
    //LED_COUNT = len_leds;

    uint offset = pio_add_program(pio, &ws2812_program);

    ws2812_program_init(pio, sm, offset, MATRIX_LED, 800000, false);

    pio_sm_set_enabled(pio, sm, true);


    //for (int i = 0; i < 25; i++){
        //pio_sm_put_blocking(pio, sm,0x00ff0000);

    //}
    sleep_ms(100);
}

void atualizar_leds(){
    for (int i = 0; i < 25; i++){
        pio_sm_put_blocking(pio, sm,led_buffer[i]);
    }   
}

void led_rgb_blink() {
    static bool led_state = false;
    gpio_put(LED_R, led_state);
    led_state = !led_state;
}



void display_number(int num) {
/*
24,23,22,21,20
15,16,17,18,19
14,13,12,11,10
05,06,07,08,09
04,03,02,01,00
*/

    // Definição de padrões para números 0-9 (matriz 5x5)
    const uint32_t numbers[10][25] = {
        { 0, 1, 1, 1, 0,
          0, 1, 0, 1, 0,
          0, 1, 0, 1, 0,
          0, 1, 0, 1, 0,
          0, 1, 1, 1, 0 }, // 0
        
        { 0, 1, 1, 1, 0,
          0, 0, 1, 0, 0,
          0, 0, 1, 0, 0,
          0, 1, 1, 0, 0,
          0, 0, 1, 0, 0 }, // 1
        
        { 0, 1, 1, 1, 0,
          0, 1, 0, 0, 0,
          0, 1, 1, 1, 0,
          0, 0, 0, 1, 0,
          0, 1, 1, 1, 0 }, // 2
        
        { 0, 1, 1, 1, 0,
          0, 0, 0, 1, 0,
          0, 1, 1, 1, 0,
          0, 0, 0, 1, 0,
          0, 1, 1, 1, 0}, // 3

        { 0, 1, 0, 0, 0,
          0, 0, 0, 1, 0,
          0, 1, 1, 1, 0,    
          0, 1, 0, 1, 0,
          0, 1, 0, 1, 0 }, // 4
        
         {0, 1, 1, 1, 0,
          0, 0, 0, 1, 0,
          0, 1, 1, 1, 0,
          0, 1, 0, 0, 0,
          0, 1, 1, 1, 0},// 5
        
        { 0, 1, 1, 1, 0,
          0, 1, 0, 1, 0,
          0, 1, 1, 1, 0,
          0, 1, 0, 0, 0,
          0, 1, 1, 1, 0}, // 6
        
        { 0, 1, 0, 0, 0,
          0, 0, 0, 1, 0,
          0, 1, 0, 0, 0,
          0, 0, 0, 1, 0,
          0, 1, 1, 1, 0 }, // 7

        
        { 0, 1, 1, 1, 0,
          0, 1, 0, 1, 0,
          0, 1, 1, 1, 0,
          0, 1, 0, 1, 0,
          0, 1, 1, 1, 0}, // 8
        
        { 0, 1, 1, 1, 0,
          0, 0, 0, 1, 0,
          0, 1, 1, 1, 0,
          0, 1, 0, 1, 0,
          0, 1, 1, 1, 0 }  // 9
    };
    
    for (int i = 0; i < NUM_PIXELS; i++) {
        
        led_buffer[i] = 0x00000000; 
        if (numbers[num][i]) {
            led_buffer[i] = 0x0000aa00;  // Define cor azul para os números
        }

    
    }
}
void debounce(uint gpio, uint32_t events) {
    static uint32_t last_time = 0;
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    if (current_time - last_time > 200000){
        //Atualiza o tempo da última interrupção
        last_time = current_time;
    if (gpio_get(gpio) == 0) {  // Verifica se ainda está pressionado
        if (gpio == BUTTON_A) {
            if (number < 9) number++;
            display_number(number);
            atualizar_leds();
            printf("Número atual: %d\n", number);
        };
        if (gpio == BUTTON_B){
            if (number > 0) number--;
            display_number(number);
            atualizar_leds();
            printf("Número atual: %d\n", number);
        }
    }
    }


}

int main() {
    stdio_init_all();
    
    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);
    
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &debounce);
    iniciar_leds();
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &debounce);

    display_number(number);
    atualizar_leds();

    while (1) {
        led_rgb_blink();
        sleep_ms(200); // Pisca 5 vezes por segundo
    
    }
}
