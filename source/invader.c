#include "invader.h"

#include <stdio.h>

void init_game(game *game_environment)
{
    init_inputs();
    init_map(&game_environment->game_map);
    game_environment->game_over = false;
    game_environment->game_win = false;
}

void init_map(map *game_map)
{
    update_map();

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
    printf("\n");
}

void update_map()
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COLUMN; j++)
            map_tile[i][j] = '-';
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

void move_entity(ship *entity, direction dir)
{
    if (dir == LEFT)
    {
        entity->y_velocity = 0;
        entity->y_velocity = -1;
    }
    else if (dir == RIGHT)
    {
        entity->y_velocity = 0;
        entity->y_velocity = 1;
    }
    else
        entity->y_velocity = 0;
}

void entity_shoot(ship *entity, direction dir)
{
    if (dir == UP)
    {
        entity->laser.x_velocity = 0;
        entity->laser.x_velocity = -1;
    }
    else if (dir == DOWN)
    {
        entity->laser.x_velocity = 0;
        entity->laser.x_velocity = 1;
    }
}

bool intersect_AABB(missile laser, alien enemy)
{
    return 0;
}

void update_world(map *world)
{
    update_map();
    update_movement_system(world);
    update_combat_system(world);
    update_collision_system(world);
    update_AI_system(world);
}

void render_world()
{
    print_map();
}

void update_movement_system(map *world)
{
    world->player.y += world->player.y_velocity;
    map_tile[(int)world->player.x][(int)world->player.y] = 'S';

    for (int i = 0; i < NUM_BUNKERS; i++)
    {
        world->bunkers[i].y += world->bunkers[i].y_velocity;
        world->bunkers[i].x += world->bunkers[i].x_velocity;
        map_tile[(int)world->bunkers[i].x][(int)world->bunkers[i].y] = 'B';
    }

    for (int i = 0; i < NUM_QUEENS; i++)
    {
        world->queen[i].y += world->queen[i].y_velocity;
        world->queen[i].x += world->queen[i].x_velocity;
        map_tile[(int)world->queen[i].x][(int)world->queen[i].y] = 'Q';
    }

    for (int i = 0; i < NUM_KNIGHTS; i++)
    {
        world->knight[i].y += world->knight[i].y_velocity;
        world->knight[i].x += world->knight[i].x_velocity;
        map_tile[(int)world->knight[i].x][(int)world->knight[i].y] = 'K';
    }

    for (int i = 0; i < NUM_PAWNS; i++)
    {
        world->pawn[i].y += world->pawn[i].y_velocity;
        world->pawn[i].x += world->pawn[i].x_velocity;
        map_tile[(int)world->pawn[i].x][(int)world->pawn[i].y] = 'P';
    }


}

void update_combat_system(map *world)
{

}

void update_collision_system(map *world)
{

}

void update_AI_system(map *world)
{

}

void init_inputs()
{
    gpioptr = gpioPtr();
}
