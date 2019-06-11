#ifndef INVADER_H
#define INVADER_H

#include <stdbool.h>

#define SPEED 1
#define DAMAGE 1
#define NUM_PAWNS 10
#define NUM_KNIGHTS 5
#define NUM_QUEENS 2
#define NUM_BUNKERS 3

void init_map();

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
	int n;
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

#endif
