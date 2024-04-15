#ifndef EDITOR_H
#define EDITOR_H

#include "UARTE.h"
#include "MicroSD.h"

typedef struct Editor_Buffer_tag
{
    char Buff[16*1024];
    uint8_t ypos;
    uint16_t xpos;

}Editor_Buffer_t;

extern Editor_Buffer_t data;
extern uint8_t isCommand;
extern uint16_t i;

void comands(MicroSD_t *sd);

void edit();

void create_path(MicroSD_t *sd);


#endif