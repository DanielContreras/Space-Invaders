#include <ECS/ecs.h>
#include <ECS/engine.h>
#include <stdio.h>
#include <stdlib.h>

ECS* InitializeECS() {
    ECS* ecs = malloc(sizeof(ECS));
    ecs->engine = InitializeEngine();
    return ecs;
}

void TerminateECS(ECS* ecs) {
    TerminateEngine(ecs->engine);
    if (ecs != NULL) {
        free(ecs);
        ecs = NULL;
    }
}

void UpdateECS(Engine* engine) { Update(engine); }

