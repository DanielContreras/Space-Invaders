#ifndef ENGINE_H
#define ENGINE_H

#include <ECS/component_manager.h>
#include <ECS/entity_manager.h>
#include <ECS/system_manager.h>

typedef EntityManager EntityManager;
typedef ComponentManager ComponentManager;
typedef SystemManager SystemManager;

typedef struct Engine {
    EntityManager* entity_manager;
    ComponentManager* component_manager;
    SystemManager* system_manager;
    //void (*Update)(struct Engine*);
} Engine;

Engine* InitializeEngine();
void TerminateEngine(Engine* engine);

void GetEntityManager();
void GetComponentManager();
void GetSystemManager();

void Update(Engine* engine);

#endif  // ENGINE_H

