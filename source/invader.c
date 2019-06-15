#include "invader.h"

#include <stdio.h>
#include <wiringPi.h>
#include <stdbool.h>
#include <pthread.h>
#include "controller.h"
#include "GPIO_INIT.h"
#include "pixel.h"
#include "images.h"

//pthread_mutex_t lock;

void init_game(Game *world)
{
    init_map(&world->world);
    init_framebuffer();
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

void update_map()
{
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COLUMN; j++)
            map_tile[i][j] = '-';
    }
}

void init_player(Entity *player)
{
    player->position.x = 960;
    player->position.y = 950;
    player->health.current_health = PLAYER_HEALTH;
    player->laser.weapon.damage = DAMAGE;
    player->type = PLAYER;
}

void init_enemies(World *world)
{
    for (int i = 0, j = 0, k = 0; i < NUM_ENEMIES; i++) {
        if (i < NUM_PAWNS) {
            if (i < 10) {
                world->enemies[i].position.x = alien_initial_x + (HORIZONTAL_OFFSET * i);
                world->enemies[i].position.y = alien_initial_y + (VERTICAL_OFFSET + (30 + 20) * 5);
            } else if (i < 20) {
                world->enemies[i].position.x = alien_initial_x + (HORIZONTAL_OFFSET * (i%10));
                world->enemies[i].position.y = alien_initial_y + (VERTICAL_OFFSET + (30 + 20) * 4);
                
            } else {
                world->enemies[i].position.x = alien_initial_x + (HORIZONTAL_OFFSET * (i%20));
                world->enemies[i].position.y = alien_initial_y + (VERTICAL_OFFSET + (30 + 20) * 3);
            }
            world->enemies[i].health.current_health = PAWN_HEALTH;
            world->enemies[i].laser.weapon.damage = DAMAGE;
            world->enemies[i].type = PAWN;
        } else if (i < 50) {
            if (j < 10) {
                world->enemies[i].position.x = alien_initial_x + (HORIZONTAL_OFFSET * (i%30));
                world->enemies[i].position.y = alien_initial_y + (VERTICAL_OFFSET + (30 + 20) * 2);
            } else {
                world->enemies[i].position.x = alien_initial_x + (HORIZONTAL_OFFSET * (i%40));
                world->enemies[i].position.y = alien_initial_y + (VERTICAL_OFFSET + (30 + 20) * 1);
            }
            world->enemies[i].health.current_health = KNIGHT_HEALTH;
            world->enemies[i].laser.weapon.damage = DAMAGE;
            world->enemies[i].type = KNIGHT;
            j++;
        } else if (i >= 50) {
            world->enemies[i].position.x = alien_initial_x + (HORIZONTAL_OFFSET * (i%50));
            world->enemies[i].position.y = alien_initial_y + (VERTICAL_OFFSET);
            world->enemies[i].health.current_health = QUEEN_HEALTH;
            world->enemies[i].laser.weapon.damage = DAMAGE;
            world->enemies[i].type = QUEEN;
        }
        world->enemies[i].alive = true;
        world->enemies[i].needs_render = true;
        world->enemies[i].needs_update = true;
        world->enemies[i].dimension.width = 41;
    }
    for (int i = 0; i < 6; i++) {
        world->left_most_enemies[i] = 10 * i;
        world->right_most_enemies[i] = 10 * i + 9;
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
            entity->velocity.x = -HORIZONTAL_SPEED;
            break;
        case RIGHT:
            entity->velocity.x = HORIZONTAL_SPEED;
            break;
        case UP:
            entity->velocity.y = -VERTICAL_SPEED;
            break;
        case DOWN:
            entity->velocity.y = VERTICAL_SPEED;
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

void *updateWorld(void *arg) 
{
    
    while (1) {
        update_AI_system(arg);
        update_movement_system(arg);
        //update_combat_system(world);
        //update_collision_system(world);
        //poll_input(world);
        delay(65);
    }
    return NULL;
}

void *updateRender(void *arg) 
{
    while (1) {
        render(arg);
    }
    return NULL;
}

void update_movement_system(World *world)
{
    world->player.previous_pos = world->player.position;
    world->player.position.x += world->player.velocity.x;

    for (int i = 0; i < NUM_BUNKERS; i++) {
        world->bunkers[i].previous_pos = world->bunkers[i].position;
        world->bunkers[i].position.x += world->bunkers[i].velocity.x;
        world->bunkers[i].position.y += world->bunkers[i].velocity.y;
    }

    for (int i = 0; i < NUM_ENEMIES; i++) {
        world->enemies[i].previous_pos = world->enemies[i].position;
        world->enemies[i].position.x += world->enemies[i].velocity.x;
        world->enemies[i].position.y += world->enemies[i].velocity.y;
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
    /* vertical reset */
    for (int i = 0; i < NUM_ENEMIES; i++) {
        move_entity(&world->enemies[i], RESET_VERTICAL);
    }
    
    /* check wall collisions */
    for (int i = 0; i < 6; i++) {
        if ((world->enemies[world->right_most_enemies[i]].position.x + world->enemies[world->right_most_enemies[i]].dimension.width) >= (RIGHT_MAX)) {
            travel_right = false;
            for (int j = 0; j < NUM_ENEMIES; j++) {
                move_entity(&world->enemies[j], DOWN);
            }
        } else if ((world->enemies[world->left_most_enemies[i]].position.x) <= (LEFT_MAX)) {
            travel_right = true;
            for (int j = 0; j < NUM_ENEMIES; j++) {
                move_entity(&world->enemies[j], DOWN);
            }
        } 
    }
    
    /* move enemies right or left */
    for (int i = 0; i < NUM_ENEMIES; i++) {
        if (travel_right) {
            move_entity(&world->enemies[i], RIGHT);
            //world->enemies[i].
        } else {
            move_entity(&world->enemies[i], LEFT);
        }
    }
}

void update_combat_system(World *world)
{

}

void update_collision_system(World *world)
{

}
