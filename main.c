#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "esp_log.h"

#include "MicroSD.h"
#include "UARTE.h"
#include "Editor.h"

#define CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED
static void echo_task(void *arg);

void app_main(void)
{
    MicroSD_t micro;
    sdmmc_card_t *card = NULL;
    s_init_card(card);
    uart_ini();
    xTaskCreate(echo_task, "uart_echo_task", 2048, NULL, 10, NULL);

    //esp_err_t ret;
    // Use POSIX and C standard library functions to work with files.
    while(1)
    {
        comands(&micro);
    }
    
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
            break;
        }while(car != 'y');
        
        break;
        vTaskDelay(100000 / portTICK_PERIOD_MS);
    }        
}
