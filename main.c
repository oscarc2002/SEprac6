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
    /*
    MicroSD_t micro;
    
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
    
    char c = '\0';
    data.lastnl = 0;
    data.i = 0;
    data.maxpos = 0;
    char aux[2] = {0};
    uint8_t len = 0;

    do
    {
        c = UART_getchar();
        
        if(c > 32)
        {
            UART_putchar(c);
            if(data.i == data.maxpos)
                data.maxpos++;
        }
        if(c == 8)
        {
            UART_putchar(8);
            UART_puts(&data.Buff[data.i]);

            if(data.i == data.maxpos)
            {
                UART_putchar(' ');
                UART_putchar(8);
                
            }
            else
            {
                UART_putchar(' ');
                gotox(data.i);
            }
//01345
            if(data.i > 1)
                data.i--;

            if(data.maxpos > 1)
                data.maxpos--;
            data.Buff[data.maxpos] = 0;
            
        }
        if(c == 27)
        {
            len = uart_read_bytes(UART_NUM_0, (void *)aux, 2, 20 / portTICK_PERIOD_MS);
            if(len > 0)
            {
                if(!strcmp("D", aux))
                {
                    if(data.i > data.lastnl)
                    {
                        data.i--;
                        UART_puts("\033[D");
                    }
                }
                else if(!strcmp("C", aux))
                {
                    if(data.i < data.maxpos)
                    {
                        data.i++;
                        UART_puts("\033[C");
                    }
                }
                memset(aux, 0, 2);
                c = 0;
            }
            
        }
        if(c > 32)
            data.Buff[data.i++] = c;
    
    }while(c != 27);
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
    */while (1)
    {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    
}