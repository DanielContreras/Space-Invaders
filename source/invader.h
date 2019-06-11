#ifndef INVADER_H
#define INVADER_H

#include <stdbool.h>

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

static char map_tile[ROW][COLUMN];

typedef struct
{
    float x, y;
    float damage;
} missile;

typedef struct
{
    float x, y;
    float speed;
    float health;
    missile laser;
    float score;
} ship;

typedef struct
{
    float x, y;
    float speed;
    float health;
    missile laser;
    int points;
} alien;

typedef struct
{
    float x, y;
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

void init_game(game *game_environment);
void init_map(map *game_map);
void init_player(ship *player);
void init_pawns(alien pawn[]);
void init_knights(alien knight[]);
void init_queens(alien queen[]);
void init_bunkers(bunker bunkers[]);
void print_map();

#endif //INVADER_H
