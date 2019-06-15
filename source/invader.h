#ifndef INVADER_H
#define INVADER_H

#include <stdbool.h>

#define DAMAGE 1

#define PLAYER_HEALTH 5

#define NUM_PAWNS 30
#define PAWN_POINTS 5
#define PAWN_HEALTH 2

#define NUM_KNIGHTS 20
#define KNIGHT_POINTS 10
#define KNIGHT_HEALTH 4

#define NUM_QUEENS 10
#define QUEEN_POINTS 100
#define QUEEN_HEALTH 15

#define NUM_BUNKERS 4
#define BUNKER_HEALTH 50

#define ROW 1920
#define COLUMN 1080

#define alien_initial_y 162
#define alien_initial_x 675
#define HORIZONTAL_OFFSET 61
#define VERTICAL_OFFSET 61
#define LEFT_MAX 150
#define RIGHT_MAX 1800
#define TOP_MAX 162
#define BOTTOM_MAX 917
#define HORIZONTAL_SPEED 4
#define VERTICAL_SPEED 4

#define NUM_ENEMIES (NUM_PAWNS + NUM_KNIGHTS + NUM_QUEENS)

char map_tile[ROW][COLUMN];
static bool travel_right = true;
unsigned int *gpio;

typedef struct
{
    float x, y;
} Position;

typedef struct
{
    float x, y;
} Velocity;

typedef struct
{
    float width, height;
} Dimension;

typedef struct
{
    float damage;
    float range;
    long cool_down;
    long last_attack;
} Weapon;

typedef union
{
    int current_health;
} Health;

typedef enum
{
    PLAYER = 1,
    PAWN = 2,
    KNIGHT = 3,
    QUEEN = 4,
    BUNKER = 5
} Type;

typedef struct
{
    Position position;
    Velocity velocity;
    Dimension dimension;
    Weapon weapon;
    bool needs_update;
    bool needs_render;
} Missile;

/* ship, alien, bunker */
typedef struct
{
    Position position;
    Position previous_pos;
    Velocity velocity;
    Dimension dimension;
    Health health;
    Missile laser;
    Type type;
    bool needs_update;
    bool needs_render;
    bool alive;
} Entity;

typedef struct map
{
    Entity player;
    Entity bunkers[NUM_BUNKERS];
    Entity enemies[NUM_ENEMIES];
    int left_most_enemies[6];
    int right_most_enemies[6];
} World;

typedef struct
{
    World world;
    bool game_win;
    bool game_over;
} Game;

typedef enum
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    STOP,
    RESET_VERTICAL,
    RESET_HORIZONTAL
} Direction;

void init_game(Game *world);
void init_map(World *world);

void init_player(Entity *player);
void init_enemies(World *world);
void init_bunkers(Entity bunkers[]);

bool intersect_AABB(Weapon laser, Entity enemy);

void *updateWorld(void *arg);
void *updateRender(void *arg);
void *clearRender(void *arg);

void move_entity(Entity *player, Direction direction);
void entity_shoot(Entity *player, Direction direction);

void update_movement_system(World *world);
void update_combat_system(World *world);
void update_collision_system(World *world);
void update_AI_system(World *world);

void poll_input(World *world);

#endif //INVADER_H
