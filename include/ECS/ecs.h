#ifndef ECS_H
#define ECS_H

#include <ECS/engine.h>

typedef struct {
    Engine* engine;
} ECS;

ECS* InitializeECS();
void TerminateECS(ECS* ecs);
void UpdateECS(Engine* engine);

#endif // ECS_H
