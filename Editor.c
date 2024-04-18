#include "Editor.h"

uint8_t isCommand = 0;
Editor_Buffer_t data = {0};

void comands(MicroSD_t *sd)
{
    while(1)
    {
        UART_gets();
        esp_err_t ret;
    
        if(!strcmp(Buffer,":u"))
        {
            char c = '\n';
            data.i = 0;
            data.lastnl = 0;

            do
            {

                c = UART_getchar();
                UART_putchar(c);
                
                if(c == '\n')
                {
                    data.lastnl = data.i;
                }
                
                data.Buff[data.i++] = c;
                
            }while(c != 27);
    
            data.Buff[--data.i] = '\0';
        }
        else if(!strcmp(Buffer,":o"))
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
        else if(!strcmp(Buffer,":e")) //UNFINISHED
        {
            char c = '\0';
            data.i = 0;
            gotoxy(0,0);
            do
            {
                c = UART_getchar();
                UART_putchar(c);
    
                
            }while(c != 27);
    
        }
        else if(!strcmp(Buffer,":n"))
        {
            UART_gets();
            strcpy(sd->Name_file, Buffer);
        }
        else if(!strcmp(Buffer,":s"))
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
    data.i = 0;
    char aux[2] = {0};
    do
    {
        c = UART_getchar();
        
        if(c > 32)
        {
            UART_putchar(c);

        }
        if(c == 27)
        {
            uart_read_bytes(UART_NUM_0, (void *)aux, 2, 20 / portTICK_PERIOD_MS);
            
            if(!strcmp("D", aux))
            {
                UART_puts("\033[D");
            }
            memset(aux, 0, 2);
        }
        data.Buff[data.i++] = c;
    }while(c != 27);
}


void create_path(MicroSD_t *sd)
{
    *sd->Path = 0;
    strcat(sd->Path, sd->Mount_Point);
    strcat(sd->Path, sd->Name_file);
}

void init_EditorBuffer(void)
{
    data.i = 0;
    data.lastnl = 0;
    data.maxpos = 0;
    *data.Buff = 0;
}