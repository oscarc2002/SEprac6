#ifndef UARTE_H
#define UARTE_H

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

//static const char *TAGUART = "MENSAJE";

char Buffer[1024];
uint16_t lenfrase;

static void uart_ini()
{
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    // Configure UART parameters
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_0, &uart_config));

    // Set UART pins
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_0, UART_TX_PIN, UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    // Setup UART buffered IO with event queue
    QueueHandle_t uart_queue;
    // Install UART driver using an event queue here
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_0, BUF_SIZE * 2, \
                                            BUF_SIZE * 2, 10, &uart_queue, 0));
}

static void UART_putchar(uint8_t data)
{
    uart_write_bytes(UART_NUM_0, (const char *) &data, 1);
}

static void UART_puts(char* text)
{
    while(*text != 0)
    {
        UART_putchar(*text);
        text++;
    }
}


static char UART_getchar()
{
    uint8_t data = 0, len;
    do
    {
        len = uart_read_bytes(UART_NUM_0, &data, 1, 20 / portTICK_PERIOD_MS);        
    }while(len == 0);

    return data;
}

static void UART_gets()
{
    int i;
    char data;
    
    for(i = 0; ; i++)
    {
        data = UART_getchar();
        UART_putchar(data);

        if(data == 8)
        {
            i-=2;
        }
        else
        {
            Buffer[i] = data;
        }
        
        if(data == 13) //Salir
        {
            Buffer[i] = '\0';
            break;
        }        
    }
    
    lenfrase = i;
}

static void echo_task(void *arg)
{
    // Configure a temporary buffer for the incoming data
    uint8_t data;
    while (1) {
        UART_puts("\nEscoge una opcion: \n1)Retador \n2)Retado\n");
        
        // Select mode
        data = UART_getchar();
        UART_putchar(data); 

                char car = '\0';
                do
                {
                    data = UART_getchar();
                    UART_putchar(data);
                    
                }while(car != 'y');
                
                break;
    }        
}

void UART_nowarning(void)
{
    uart_ini();
    UART_putchar('0');
    UART_getchar();
    UART_gets();
    echo_task((void *)NULL);
}
#endif