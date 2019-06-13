#include "invader.h"

#include <stdio.h>
#include <wiringPi.h>
#include "controller.h"
#include "GPIO_INIT.h"
#include "pixel.h"

void init_game(Game *world)
{
    init_map(&world->world);
    world->game_over = false;
    world->game_win = false;
}

void init_map(World *world)
{
    update_map();
    init_player(&world->player);
    init_enemies(world);
    init_bunkers(world->bunkers);
}

void print_map() {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COLUMN; j++)
            printf("%c ", map_tile[i][j]);
        printf("\n");
    }
    printf("\n");
}

void update_map()
{
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COLUMN; j++)
            map_tile[i][j] = '-';
    }
}

void init_player(Entity *player)
{
    player->position.x = 20;
    player->position.y = 24;
    player->health.current_health = PLAYER_HEALTH;
    player->laser.weapon.damage = DAMAGE;
    player->type = PLAYER;
    map_tile[(int) player->position.y][(int) player->position.x] = 'S';
}

void init_enemies(World *world)
{
    for (int i = 0, j = 0, k = 0; i < NUM_ENEMIES; i++) {
        if (i < 30) {
            if (i < 10) {
                world->enemies[i].position.x = alien_initial_x + i*2;
                world->enemies[i].position.y = alien_initial_y + 3;
            } else if (i < 20) {
                world->enemies[i].position.x = alien_initial_x + (i%10)*2;
                world->enemies[i].position.y = alien_initial_y + 4;
            } else {
                world->enemies[i].position.x = alien_initial_x + (i%20)*2;
                world->enemies[i].position.y = alien_initial_y + 5;
            }
            world->enemies[i].health.current_health = PAWN_HEALTH;
            world->enemies[i].laser.weapon.damage = DAMAGE;
            world->enemies[i].type = PAWN;
            map_tile[(int) world->enemies[i].position.y][(int) world->enemies[i].position.x] = 'P';
        } else if (i < 50) {
            if (j < 10) {
                world->enemies[i].position.x = alien_initial_x + j*2;
                world->enemies[i].position.y = alien_initial_y + 1;
            } else {
                world->enemies[i].position.x = alien_initial_x + (j%10)*2;
                world->enemies[i].position.y = alien_initial_y + 2;
            }
            world->enemies[i].health.current_health = KNIGHT_HEALTH;
            world->enemies[i].laser.weapon.damage = DAMAGE;
            world->enemies[i].type = KNIGHT;
            map_tile[(int) world->enemies[i].position.y][(int) world->enemies[i].position.x] = 'K';
            j++;
        } else if (i >= 50) {
            world->enemies[i].position.x = alien_initial_x + (i%50)*2;
            world->enemies[i].position.y = alien_initial_y;
            world->enemies[i].health.current_health = QUEEN_HEALTH;
            world->enemies[i].laser.weapon.damage = DAMAGE;
            world->enemies[i].type = QUEEN;
            map_tile[(int) world->enemies[i%50].position.y][(int) world->enemies[i%50].position.x] = 'Q';
        }
    }
}

void init_bunkers(Entity bunkers[])
{
    for (int i = 0; i < NUM_BUNKERS; i++) {
        bunkers[i].position.x = 17 + i + i;
        bunkers[i].position.y = 22;
        bunkers[i].health.current_health = BUNKER_HEALTH;
        bunkers[i].type = BUNKER;
        map_tile[(int) bunkers[i].position.y][(int) bunkers[i].position.x] = 'B';
    }
}

void move_entity(Entity *entity, Direction direction)
{
    switch (direction) {
        case LEFT:
            entity->velocity.x = 0;
            entity->velocity.x = -1;
            break;
        case RIGHT:
            entity->velocity.x = 0;
            entity->velocity.x = 1;
            break;
        case UP:
            entity->velocity.y = -1;
            break;
        case DOWN:
            entity->velocity.y = 1;
            break;
        case RESET_VERTICAL:
            entity->velocity.y = 0;
            break;
        case RESET_HORIZONTAL:
            entity->velocity.x = 0;
            break;
        default:
            entity->velocity.x = 0;
            entity->velocity.y = 0;
    }
}

void entity_shoot(Entity *entity, Direction direction)
{

}

void update_world(World *world)
{
    update_map();
    update_AI_system(world);
    update_movement_system(world);
    update_combat_system(world);
    update_collision_system(world);
//    poll_input(world);
}

void render_world(World *world)
{
    print_map();
}

void update_movement_system(World *world)
{
    world->player.position.x += world->player.velocity.x;
    map_tile[(int) world->player.position.y][(int) world->player.position.x] = 'S';

    for (int i = 0; i < NUM_BUNKERS; i++) {
        world->bunkers[i].position.x += world->bunkers[i].velocity.x;
        world->bunkers[i].position.y += world->bunkers[i].velocity.y;
        map_tile[(int) world->bunkers[i].position.y][(int) world->bunkers[i].position.x] = 'B';
    }

    for (int i = 0; i < NUM_ENEMIES; i++) {
        world->enemies[i].position.x += world->enemies[i].velocity.x;
        world->enemies[i].position.y += world->enemies[i].velocity.y;
        if (i < 30) {
            map_tile[(int) world->enemies[i].position.y][(int) world->enemies[i].position.x] = 'P';
        } else if (i < 50) {
            map_tile[(int) world->enemies[i].position.y][(int) world->enemies[i].position.x] = 'K';
        } else if (i >= 50) {
            map_tile[(int) world->enemies[i].position.y][(int) world->enemies[i].position.x] = 'Q';
        }
    }
}

void poll_input(World *world)
{
//    input = read_snes();
//    switch (input) {
//        case 's':
//            printf("You have pressed B\n");
//            entity_shoot(&world->player, UP);
//            break;
//        case 2:
//            printf("You have pressed Y\n");
//            break;
//        case 3:
//            printf("You have pressed Select\n");
//            break;
//        case 5:
//            printf("You have pressed Joy-pad UP\n");
//            break;
//        case 6:
//            printf("You have pressed Joy-pad DOWN\n");
//            break;
//        case 'a':
//            printf("You have pressed Joy-pad LEFT\n");
//            move_entity(&world->player, LEFT);
//            break;
//        case 'd':
//            printf("You have pressed Joy-pad RIGHT\n");
//            move_entity(&world->player, RIGHT);
//            break;
//        case 9:
//            printf("You have pressed A\n");
//            entity_shoot(&world->player, UP);
//            break;
//        case 10:
//            printf("You have pressed X\n");
//            break;
//        case 11:
//            printf("You have pressed LEFT\n");
//            break;
//        case 12:
//            printf("You have pressed RIGHT\n");
//            break;
//        default:
//            move_entity(&world->player, STOP);
//    }
}

void update_AI_system(World *world)
{
    for (int i = 0; i < NUM_ENEMIES; i++) {
        move_entity(&world->enemies[i], RESET_VERTICAL);
        if (travel_right) {
            if (world->enemies[NUM_ENEMIES - 1].position.x == (COLUMN - 2)) {
                travel_right = false;
                for (int j = 0; j < NUM_ENEMIES; j++)
                    move_entity(&world->enemies[j], DOWN);
                break;
            }
            move_entity(&world->enemies[i], RIGHT);
        } else {
            if (world->enemies[10].position.x == 1) {
                travel_right = true;
                for (int j = 0; j < NUM_ENEMIES; j++)
                    move_entity(&world->enemies[j], DOWN);
                break;
            }
            move_entity(&world->enemies[i], LEFT);
        }
    }
    /* Randomly engage enemies in combat here */
}

void update_combat_system(World *world)
{

}

void update_collision_system(World *world)
{

}
