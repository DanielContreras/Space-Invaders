#ifndef COMPONENT_H
#define COMPONENT_H

typedef struct Component {
    unsigned int component_id;
    unsigned int owner_id;
    bool enabled;
} Component;

typedef struct PositionComponent {
    Component component;
    float x;
    float y;
} PositionComponent;

typedef struct VelocityComponent {
    Component component;
    float x_velocity;
    float y_velocity;
} VelocityComponent;

typedef struct DimensionComponent {
    Component component;
    int width;
    int height;
} DimensionComponent;

typedef struct HealthComponent {
    Component component;
    float max_hp;
    float current_hp;
} HealthComponent;

void GetComponentID();
void GetOwnedID();
void SetActive();
void IsActive();

#endif  // COMPONENT_H
