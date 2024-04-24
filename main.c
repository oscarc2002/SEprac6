#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "esp_log.h"

#include "UARTE.h"
#include "Editor.h"
#include "MicroSD.h"

#define CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED

void app_main(void)
{   
    MicroSD_t micro;
    isCommand = 1;
    uart_ini();
    gotoxy(0, 0);
    UART_puts("\033[1;100r");
    vTaskDelay(100 / portTICK_PERIOD_MS);
    init_MicroSD(&micro);
    init_EditorBuffer();
    UART_puts("\033[1;25r");

    while (1)
    {
        switch (isCommand)
        {
        case 0:
            edit();
            break;
        
        default:
            comands(&micro);
            break;
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
