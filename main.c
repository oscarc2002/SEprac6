#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "esp_log.h"

#include "UARTE.h"
#include "Editor.h"
#include "MicroSD.h"


void app_main(void)
{   
    MicroSD_t micro;
    isCommand = 1;
    init_EditorBuffer();
    uart_ini();
    gotoxy(0, 0);
    UART_puts("\033[1;100r");
    UART_puts("\033[1;25r");
    init_MicroSD(&micro);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    getchar();
    clrscr();

    while (1)
    {
        if(!(isCommand & (1 << 0)))
        {
             edit();
        }   
        else
        {
            comands(&micro);
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
