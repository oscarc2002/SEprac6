#ifndef UARTE_H_
#define UARTE_H_

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include <string.h>

#define BUF_SIZE        (1024)
#define UART_RX_PIN     (3)
#define UART_TX_PIN     (1)

//const char *TAGUART = "MENSAJE";

extern char Buffer[1024];
extern uint16_t lenfrase;

void uart_ini();

void UART_putchar(uint8_t data);

void UART_puts(char *text);

char UART_getchar();

void UART_gets();

void clrscr(void);

void gotoxy(uint8_t x, uint8_t y);

void gotox(uint8_t x);
#endif