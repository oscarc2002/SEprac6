#include "Editor.h"

uint8_t isCommand;
Editor_Buffer_t data;
uint16_t i;

void comands(MicroSD_t *sd)
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
                UART_putchar(c);
    
                data.Buff[i++] = c;
            }while(c != 27);
    
            data.Buff[--i] = '\0';
        }
        else if(strcmp(Buffer,":o"))
        {
            
            UART_gets();
            
            strcpy(sd->Name_file, Buffer);
            create_path(sd);
            clrscr(); //Clean screen
            ret = s_example_read_file(sd->Path);
            if (ret != ESP_OK) {
                return;
            }
        }
        else if(strcmp(Buffer,":e")) //UNFINISHED
        {
            char c = '\0';
            i = 0;
            gotoxy(0,0);
            do
            {
                c = UART_getchar();
                UART_putchar(c);
    
                
            }while(c != 27);
    
        }
        else if(strcmp(Buffer,":n"))
        {
            UART_gets();
            strcpy(sd->Name_file, Buffer);
        }
        else if(strcmp(Buffer,":s"))
        {
            //Check if it has a name
            if(sd->Name_file[0] == '\0') //If is in blank
                strcpy(sd->Name_file, "default.txt");
            
            create_path(sd);
            ret = s_example_write_file(sd->Path, data.Buff);
        }
        else
        {
            ESP_LOGI("Error:", "comando no valido");
        }
    }
}

void edit()
{
    gotoxy(0,0);
    char c = '\0';
    i = 0;
    do
    {
        c = UART_getchar();
        UART_putchar(c);

        //Backspace,derecha,izquierda

        data.Buff[i++] = c;
    }while(c != 27);
}


void create_path(MicroSD_t *sd)
{
    *sd->Path = 0;
    strcat(sd->Path, sd->Mount_Point);
    strcat(sd->Path, sd->Name_file);
}
