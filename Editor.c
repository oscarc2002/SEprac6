#ifndef EDITOR_H
#define EDITOR_H

#include "MicroSD.h"
#include "UARTE.c"
uint8_t isCommand;

typedef struct Editor_Buffer_tag
{
    char Buff[16][1024];
    uint8_t ypos;
    uint16_t xpos;

}Editor_Buffer_t;

static void comands(MicroSD_t *sd)
{
    UART_gets();
    esp_err_t ret;

    if(strcmp(Buffer,":u"))
    {
        char c = '\0';
        do
        {
            c = UART_getchar();
            UART_putchar(c);
        }while(c != 27);
    }
    else if(strcmp(Buffer,":o"))
    {
        UART_gets();
        strcpy(Buffer, sd->Name_file);
        create_path(sd);
        ret = s_example_read_file(sd->Path);
        if (ret != ESP_OK) {
            return;
        }
    }
    else if(strcmp(Buffer,":e"))
    {
        
    }
    else if(strcmp(Buffer,":n"))
    {
        char name[32] = {0};
        uint8_t i = 0;
        do
        {
            name[i] = UART_getchar();
            UART_putchar(name[i]);
        }while(name[i++] != 27);
        strcpy(sd->Name_file, name);
    }
    else if(strcmp(Buffer,":s"))
    {

    }
    else
    {
        ESP_LOGI("Error:", "comando no valido");
    }
}

static void edit()
{
    return;
}

void create_path(MicroSD_t *sd)
{
    *sd->Path = 0;
    strcat(sd->Path, sd->Mount_Point);
    strcat(sd->Path, sd->Name_file);
}

void EDITOR_nowarning(void)
{
    MicroSD_t a;
    comands(&a);
    edit();
}
#endif
