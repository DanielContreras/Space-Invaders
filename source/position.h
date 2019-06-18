#ifndef POSITION_H
#define POSITION_H

typedef struct Position
{
    float x, y;
} Position;

typedef struct Dimension
{
    float width, height;
} Dimension;

typedef struct Velocity
{
    float x, y;
} Velocity;

typedef union Health
{
    int current_health;
} Health;

typedef enum Type
{
    PLAYER = 1,
    PAWN = 2,
    KNIGHT = 3,
    QUEEN = 4,
    BUNKER = 5
} Type;

typedef enum Direction
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    STOP,
    RESET_VERTICAL,
    RESET_HORIZONTAL
} Direction;

#endif

