#ifndef EDITOR_H
#define EDITOR_H

#include "MicroSD.c"
#include "UARTE.c"
uint8_t isCommand;

typedef struct Editor_Buffer_tag
{
    char Buff[16*1024];
    uint16_t pos;

}Editor_Buffer_t;

Editor_Buffer_t data;
uint16_t i;
char name[32] = {0};

static void comands(MicroSD_t *sd)
{
    while(1)
    {
        UART_gets();
        esp_err_t ret;
    
        if(strcmp(Buffer,":u"))
        {
            char c = '\0';
            i = 0;
            do
            {
                c = UART_getchar();
                if(c == 13)
                {
                    UART_putchar('\n');
                    data.pos = i;
                }
                else
                    UART_putchar(c);
    
                data.Buff[i++] = c;
            }while(c != 27);
    
            data.Buff[--i] = '\0';
        }
        else if(strcmp(Buffer,":o"))
        {
            do
            {
                UART_gets();
            }while(Buffer[0] == '\0');
            
            strcpy(Buffer, sd->Name_file);
            create_path(sd);
            clrscr(); //Clean screen
            ret = s_example_read_file(sd->Path, data.Buff);
            if (ret != ESP_OK) {
                return;
            }
        }
        else if(strcmp(Buffer,":e"))
        {
            
        }
        else if(strcmp(Buffer,":n"))
        {
            uint8_t j = 0;
            do
            {
                name[j] = UART_getchar();
                UART_putchar(name[j]);
            }while(name[j++] != 27);
            name[--j] = '\0';
        }
        else if(strcmp(Buffer,":s"))
        {
            //Check if it has a name
            if(name[0] == '\0') //If is in blank
                strcpy(sd->Name_file, "default.txt");
            else
                strcpy(sd->Name_file, name);
            
            create_path(sd);
            ESP_LOGI(TAG, "Abriendo archivo %s", sd->Path);
            FILE *f = fopen(sd->Path, "w");
            if (f == NULL) {
                ESP_LOGE(TAG, "Fallo abrir el archivo para escritura");
                return ESP_FAIL;
            }
            fprintf(f, data);
            fclose(f);
            ESP_LOGI(TAG, "Archivo escrito");
        }
        else
        {
            ESP_LOGI("Error:", "comando no valido");
        }
    }
}

static void edit()
{
    gotoxy(0,0);
    char c = '\0';
    i = 0;
    do
    {
        c = UART_getchar();
        if(c == 13) //obsolete
        {
            UART_putchar('\n');
            data.pos = i;
        }
        else
            UART_putchar(c);

        data.Buff[i++] = c;
    }while(c != 27);
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
