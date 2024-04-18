#ifndef EDITOR_H
#define EDITOR_H

#include "MicroSD.h"

typedef struct Editor_Buffer_tag
{
    char Buff[16*1024];
    uint16_t lastnl;
    uint16_t i;
    uint16_t maxpos;

}Editor_Buffer_t;

extern Editor_Buffer_t data;
extern uint8_t isCommand;

void comands(MicroSD_t *sd);

void edit();

void create_path(MicroSD_t *sd);

void init_EditorBuffer(void);

void back_array(Editor_Buffer_t *data);

#endif
