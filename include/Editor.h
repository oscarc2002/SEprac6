#ifndef EDITOR_H
#define EDITOR_H

#include<string.h>
#include "MicroSD.h"
#define CMDLINE 27
#define NAMEFILE 26

typedef struct Editor_Buffer_tag
{
    char Buff[16*1024];
    uint16_t lastnl;
    uint16_t i;
    uint16_t maxpos;
    uint8_t xpos;
    uint8_t ypos;
}Editor_Buffer_t;

extern Editor_Buffer_t data;
extern uint8_t isCommand;

void comands(MicroSD_t *sd);

void edit();

void create_path(MicroSD_t *sd);

void init_EditorBuffer(void);

void clear_NameFile(char *name);

void back_array(void);

void front_array(void);

#endif
