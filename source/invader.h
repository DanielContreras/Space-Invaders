#ifndef INVADER_H
#define INVADER_H

#include <stdbool.h>
#include "pixel.h"

#define SPEED 1
#define DAMAGE 1

#define PLAYER_HEALTH 5

#define NUM_PAWNS 10
#define PAWN_POINTS 5
#define PAWN_HEALTH 2

#define NUM_KNIGHTS 5
#define KNIGHT_POINTS 10
#define KNIGHT_HEALTH 4

#define NUM_QUEENS 2
#define QUEEN_POINTS 100
#define QUEEN_HEALTH 15

#define NUM_BUNKERS 4

#define ROW 25
#define COLUMN 25
#define MAX_MISSILES 100

char map_tile[ROW][COLUMN];
unsigned int *gpio;

typedef struct
{
    float x, y;
    float x_velocity, y_velocity;
    float width, height;
    float health;
    bool isPlayer;
    bool needs_update;
    bool needs_render;
} entity;

typedef struct
{
    float x, y;
    float x_velocity;
    float width, height;
    float damage;
} missile;

typedef struct
{
    float x, y;
    float y_velocity;
    float speed;
    float width, height;
    float health;
    missile laser;
    float score;
    render_image image;
} ship;

typedef struct
{
    float x, y;
    float x_velocity, y_velocity;
    float speed;
    float width, height;
    float health;
    missile laser;
    int points;
} alien;

typedef struct
{
    float x, y;
    float x_velocity, y_velocity;
    float width, height;
    float health;
} bunker;

typedef struct
{
    ship player;
    alien pawn[NUM_PAWNS];
    alien knight[NUM_KNIGHTS];
    alien queen[NUM_QUEENS];
    bunker bunkers[NUM_BUNKERS];
} map;

typedef struct
{
    map game_map;
    bool game_win;
    bool game_over;
} game;

typedef enum
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    NONE
} direction;

void init_game(game *game_environment);
void init_map(map *game_map);
void init_inputs();
void update_map();
void print_map();

void init_player(ship *player);
void init_pawns(alien pawn[]);
void init_knights(alien knight[]);
void init_queens(alien queen[]);
void init_bunkers(bunker bunkers[]);

bool intersect_AABB(missile laser, alien enemy);

void update_world(map *world);
void render_world(map *world);

void move_entity(ship *player, direction dir);
void entity_shoot(ship *player, direction dir);

void update_movement_system(map *world);
void update_combat_system(map *world);
void update_collision_system(map *world);
void update_AI_system(map *world);

void poll_input(map *world);

#endif //INVADER_H
