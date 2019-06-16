#ifndef ENTITY_H
#define ENTITY_H

#include <stdbool.h>
#include "position.h"

typedef struct Entity
{
    unsigned int id;
    Position position;
    Position previous_pos;
} Entity;

void add_entity();
void remove_entity();

#endif

