#ifndef ENTITY_H
#define ENTITY_H

typedef struct Entity {
    unsigned int entity_id;
    bool active;
} Entity;

void GetEntityID();
void SetActive();
void IsActive();
void OnEnable();
void OnDisable();

void GetComponent();
void AddComponent();
void RemoveComponent();

#endif // ENTITY_H
