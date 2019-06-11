#include "invader.h"

#include <stdio.h>

void init_game(game *game_environment)
{
    init_map(&game_environment->game_map);
    game_environment->game_over = false;
    game_environment->game_win = true;
}

void init_map(map *game_map)
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COLUMN; j++)
            map_tile[i][j] = '-';
    }

    init_player(&game_map->player);
    init_pawns(game_map->pawn);
    init_knights(game_map->knight);
    init_queens(game_map->queen);
    init_bunkers(game_map->bunkers);

    print_map();
}

void print_map()
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COLUMN; j++)
            printf("%c ", map_tile[i][j]);
        printf("\n");
    }
}

void init_player(ship *player)
{
    player->x = 24;
    player->y = 12;
    player->speed = SPEED;
    player->health = PLAYER_HEALTH;
    player->laser.damage = DAMAGE;
    map_tile[(int)player->x][(int)player->y] = 'S';
}

void init_pawns(alien pawn[])
{
    for (int i = 0; i < NUM_PAWNS; i++)
    {
        pawn[i].x = 2;
        pawn[i].y = 3 + i + i;
        pawn[i].speed = SPEED;
        pawn[i].health = PAWN_HEALTH;
        pawn[i].laser.damage = DAMAGE;
        pawn[i].points = PAWN_POINTS;
        map_tile[(int)pawn[i].x][(int)pawn[i].y] = 'P';
    }
}

void init_knights(alien knight[])
{
    for (int i = 0; i < NUM_KNIGHTS; i++)
    {
        knight[i].x = 1;
        knight[i].y = 8 + i + i;
        knight[i].speed = SPEED;
        knight[i].health = KNIGHT_HEALTH;
        knight[i].laser.damage = DAMAGE;
        knight[i].points = KNIGHT_POINTS;
        map_tile[(int)knight[i].x][(int)knight[i].y] = 'K';
    }
}

void init_queens(alien *queen)
{
    for (int i = 0; i < NUM_QUEENS; i++)
    {
        queen[i].x = 0;
        queen[i].y = 11 + i + i;
        queen[i].speed = SPEED;
        queen[i].health = QUEEN_HEALTH;
        queen[i].laser.damage = DAMAGE;
        queen[i].points = QUEEN_POINTS;
        map_tile[(int)queen[i].x][(int)queen[i].y] = 'Q';
    }
}

void init_bunkers(bunker bunkers[])
{
    for (int i = 0; i < NUM_BUNKERS; i++)
    {
        bunkers[i].x = 22;
        bunkers[i].y = 9 + i + i;
        bunkers[i].health = 50;
        map_tile[(int)bunkers[i].x][(int)bunkers[i].y] = 'B';
    }
}
