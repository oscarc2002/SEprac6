#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "MicroSD.c"
#include "UARTE.c"
#include "Editor.c"

#define CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED

void app_main(void)
{
    MicroSD_t micro;
    sdmmc_card_t *card = NULL;
    s_init_card(card);
    uart_ini();
    xTaskCreate(echo_task, "uart_echo_task", 2048, NULL, 10, NULL);

    //esp_err_t ret;
    // Use POSIX and C standard library functions to work with files.

    comands(&micro);

    while(1)
    {
        char c = UART_getchar();

        if(c == 27)
        {
            isCommand = 1;
        }
        
        if(isCommand)
            comands(&micro);
        else
            edit();
    }
    
}
