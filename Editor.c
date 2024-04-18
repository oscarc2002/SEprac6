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
    char aux[2] = {0};
    char c = '\0';
    uint8_t len = 0, cursor = strlen(&data.Buff[data.lastnl+1]);
    data.i = data.maxpos;
    
    do
    {
        c = UART_getchar();
 
        if(c >= 32)
        {
            UART_putchar(c);
            if(data.i != data.maxpos)
            {   
                front_array();
                UART_puts(&data.Buff[data.i+1]);
                gotox(cursor+2);
            }
            cursor++;
            data.Buff[data.i++] = c;
            data.maxpos++;            
            data.Buff[data.maxpos] = 0;
            
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
                gotox(cursor);
            }
    
            if(data.i > data.lastnl)
            {
                data.i--;
                cursor--;
                back_array();
            }    
            
            if(data.maxpos > data.lastnl)
            {
                data.maxpos--;
                data.Buff[data.maxpos] = 0;
            }    
            
        }
        if(c == 27)
        {
            len = uart_read_bytes(UART_NUM_0, (void *)aux, 2, 10 / portTICK_PERIOD_MS);
            if(len > 0)
            {
                if(!strcmp("D", aux))
                {
                    if(data.i > data.lastnl)
                    {
                        data.i--;
                        cursor--;
                        UART_puts("\033[D");
                    }
                }
                else if(!strcmp("C", aux))
                {
                    if(data.i < data.maxpos)
                    {
                        data.i++;
                        cursor++;
                        UART_puts("\033[C");
                    }
                }
                memset(aux, 0, 2);
                c = 0;
            }     
        }
        if(c == 27)
        {
            clrscr();
            UART_puts(data.Buff);
        }
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

void back_array(void)
{
    for(uint16_t i = data.i; i <= data.maxpos; i++)
    {
        data.Buff[i] = data.Buff[i+1];
    }
}

void front_array(void)
{
    for(uint16_t i = data.maxpos; i >= data.i; i--)
    {
        
        data.Buff[i+1] = data.Buff[i];
        
        if(i == 0)
        {
            break;
        }
    }
}
