#include <ECS/component_manager.h>
#include <ECS/engine.h>
#include <ECS/entity_manager.h>
#include <ECS/system_manager.h>
#include <stdio.h>
#include <stdlib.h>

void Update(Engine* engine) {
    UpdateSystemManager(engine->system_manager);
    RemoveDeletedEntities(engine->entity_manager);
}

Engine* InitializeEngine() {
    Engine* engine = NULL;
    engine = malloc(sizeof(Engine));
    engine->entity_manager = malloc(sizeof(EntityManager));
    engine->component_manager = malloc(sizeof(ComponentManager));
    engine->system_manager = malloc(sizeof(SystemManager));
    return engine;
}

void TerminateEngine(Engine* engine) {
    if (engine->entity_manager != NULL) {
        free(engine->entity_manager);
        engine->entity_manager = NULL;
    }
    if (engine->component_manager != NULL) {
        free(engine->component_manager);
        engine->component_manager = NULL;
    }
    if (engine->system_manager != NULL) {
        free(engine->system_manager);
        engine->system_manager = NULL;
    }
    if (engine != NULL) {
        free(engine);
        engine = NULL;
    }
}
