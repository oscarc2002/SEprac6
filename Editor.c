#include "Editor.h"

uint8_t isCommand = 0;
Editor_Buffer_t data = {0};

void comands(MicroSD_t *sd)
{
    while(1)
    {
        clear_CMDLine();
        UART_gets();
        esp_err_t ret;

        if(!strcmp(Buffer,":u"))
        {
            
            char c = '\n';

            if(!(isCommand & (1 << 2)))
            {
                clrscr();
                display_CMDLine("-- INSERTAR --");
                gotoxy(0, 0);
                init_EditorBuffer();
                isCommand |= (1 << 2);
            }
            else
            {
                data.xpos = strlen(&data.Buff[data.lastnl])+1;
                gotoxy(data.xpos, data.ypos);
            }
            do
            {

                c = UART_getchar();

                if(c >= 32 || c == 13)
                {
                    UART_putchar(c);
                    data.Buff[data.i++] = c;
                    data.xpos++;
                }
                
                if(c == 13)
                {
                    data.lastnl = data.i;
                    data.xpos = 1;
                    data.ypos++;
                }
          
            }while(c != 27);
            uart_flush(UART_NUM_0);
            data.Buff[data.i] = '\0';
            data.maxpos = data.i;
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
            isCommand = 0;
            break;
        }
        else if(!strcmp(Buffer,":n"))
        {
            UART_gets();
            strcpy(sd->Name_file, Buffer);
        }
        else if(!strcmp(Buffer,":s"))
        {
            clrscr();
            UART_puts(data.Buff);
            /*
            //Check if it has a name
            if(sd->Name_file[0] == '\0') //If is in blank
                strcpy(sd->Name_file, "default.txt");
            
            create_path(sd);
            ret = s_example_write_file(sd->Path, data.Buff);*/
        }
        else
        {
            uart_flush(UART_NUM_0);
            display_CMDLine("-- Comando no valido --");
            UART_getchar();
        }
    }
}

void edit()
{
    char aux[2] = {0};
    char c = '\0';
    uint8_t len = 0;

    display_CMDLine("-- EDICION --");
    gotoxy(data.xpos, data.ypos);

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
                gotox(data.xpos+1);
            }
            data.xpos++;
            data.Buff[data.i++] = c;
            data.Buff[++data.maxpos] = 0;
            
        }
        if(c == 8)
        {
            UART_putchar(8);
            UART_puts(&data.Buff[data.i]);
            UART_putchar(' ');

            if(data.i == data.maxpos)
            {
                UART_putchar(8);
                
            }
            else
            {
                gotox(data.xpos-1);
            }
    
            if(data.i > data.lastnl)
            {
                data.i--;
                data.xpos--;
                back_array();
            }    
            
            if(data.maxpos > data.lastnl)
            {
                data.Buff[--data.maxpos] = 0;
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
                        data.xpos--;
                        UART_puts("\033[D");
                    }
                }
                else if(!strcmp("C", aux))
                {
                    if(data.i < data.maxpos)
                    {
                        data.i++;
                        data.xpos++;
                        UART_puts("\033[C");
                    }
                }
                memset(aux, 0, 2);
                c = 0;
            }     
        }
        
    }while(c != 27);
    isCommand = 1;
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
    data.xpos = 1;
    data.ypos = 1;
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
