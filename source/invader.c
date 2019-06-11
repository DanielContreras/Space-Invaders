#include "invader.h"

#include <stdio.h>

void init_map()
{
	map game_map = {.player.speed = SPEED, .player.health = 5, .player.laser.damage = DAMAGE};
		
	for (int i = 0; i < NUM_PAWNS; i++)
	{
		game_map.pawn[i].speed = SPEED;
		game_map.pawn[i].health = 2;
		game_map.pawn[i].points = 5;
		game_map.pawn[i].laser.damage = DAMAGE;
	}
	
	for (int i = 0; i < NUM_KNIGHTS; i++)
	{
		game_map.knight[i].speed = SPEED;
		game_map.knight[i].health = 4;
		game_map.knight[i].points = 10;
		game_map.knight[i].laser.damage = DAMAGE;
	}
	
	for (int i = 0; i < NUM_QUEENS; i++)
	{
		game_map.queen[i].speed = SPEED;
		game_map.queen[i].health = 15;
		game_map.queen[i].points = 100;
		game_map.queen[i].laser.damage = DAMAGE;
	}
	
	for (int i = 0; i < NUM_BUNKERS; i++)
	{
		game_map.bunkers[i].health = 50;
	}
	
	printf("%f\n", game_map.player.laser.damage);
}
