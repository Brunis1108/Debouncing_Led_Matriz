# Controle de Matriz de LEDs WS2812 com botões

## 📌 Visão Geral
Este projeto implementa um controle de matriz 5x5 de LEDs WS2812 utilizando um Raspberry Pi Pico (RP2040). Ele também permite a interação por meio de dois botões, que incrementam e decrementam números exibidos na matriz de LEDs.

## 🛠 Componentes Utilizados
- **Microcontrolador:** Raspberry Pi Pico (RP2040)
- **Matriz de LEDs:** 5x5 com LEDs WS2812 (Neopixel)
- **LED RGB:** Conectado aos GPIOs 11, 12 e 13
- **Botões:** Dois botões conectados aos GPIOs 5 e 6

## 🎯 Funcionamento

### Exibição de Números na Matriz:
- A matriz 5x5 exibe números de 0 a 9, definidos como padrões na função `display_number()`.
- O padrão de LEDs é armazenado em um buffer (`led_buffer`) e atualizado com `atualizar_leds()`.

### Controle via Botões:
- O botão **A** (GPIO 5) incrementa o número exibido.
- O botão **B** (GPIO 6) decrementa o número exibido.
- Um sistema de debouncing evita leituras falsas dos botões.

### LED RGB:
- O LED vermelho (GPIO 13) pisca a cada 200ms, indicando que o sistema está funcionando.

## 📜 Estrutura do Código

### 🔹 Principais Funções
```c
// Inicializa a comunicação com a matriz WS2812
void iniciar_leds();

// Atualiza a matriz conforme o buffer de LEDs
void atualizar_leds();

// Faz o LED vermelho piscar a cada 200ms
void led_rgb_blink();

// Define a exibição de números na matriz de LEDs
void display_number(int num);

// Trata a interrupção dos botões e altera o número exibido
void debounce(uint gpio, uint32_t events);
```

### Link do vídeo  
https://drive.google.com/file/d/1gU2e-aXjTTPz9apIHIDN8lHFspDosraZ/view?usp=drive_link
