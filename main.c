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
    MicroSD_t micro = {0};
    isCommand = 0;
    isCommand = 1;
    /*
    s_init_card();
    init_MicroSD(&micro);
    ESP_LOGI("Murio", "MURio");
    char c;
    char data[2];
    while(1){
        c = UART_getchar();
        if(c >= 32)
        {
            UART_putchar(c);
        }
        
        if(c == 27)
        {
            uart_read_bytes(UART_NUM_0, (void *)data, 2, 20 / portTICK_PERIOD_MS);
            if(!strcmp("D", data))
            {
                UART_puts("\033[D");
            }
            memset(data, 0, 2);
        }
    }*/

    uart_ini();
    init_EditorBuffer();  
    /*
    strcpy("hellow", micro.Name_file);
    ESP_LOGI("Murio", "MURio");
    create_path(&micro);
    ESP_LOGI("Murio", "MURio");
    char str[] = "Frase ";
    for (uint8_t i = 0; i < 5; i++)
    {
        str[6] = i + 48;
        s_example_write_file(micro.Path, str);
    }

    s_example_read_file(micro.Path);
    */
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
