#ifndef SYSTEM_H
#define SYSTEM_H

typedef struct System {
    unsigned int system_id;
    char* type_name;
} System;

void GetSystemID();
void GetSystemTypeName();

void PreUpdate();
void Update();
void PostUpdate();

#endif  // SYSTEM_H
