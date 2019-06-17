#include "invader.h"

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wiringPi.h>
#include "GPIO_INIT.h"
#include "controller.h"
#include "images.h"
#include "render.h"

void init_game(Game *world) {
    init_map(&world->world);
    init_framebuffer();
    gpio = gpioPtr();
    world->game_over = false;
    world->game_start = false;
    world->main_menu.on_game_menu = true;
    world->main_menu.game_start_menu = true;
    world->game_win = false;
}

void init_map(World *world) {
    init_player(&world->player);
    init_enemies(world);
    world->game_menu.game_menu_option = 0;
    world->game_menu.on_gameMenu_menu = false;
    world->game_over = false;
    init_bunkers(world->bunkers);
    init_playerScore(&world->playerScore);
    init_life(&world->life);
}

void init_player(Entity *player) {
    player->dimension.height = blue_ship_sprite.height;
    player->dimension.width = blue_ship_sprite.width;
    player->position.x = (MAP_WIDTH / 2) - (player->dimension.width / 2);
    player->position.y = MAP_HEIGHT - 162;
    player->health.current_health = PLAYER_HEALTH;
    for (int i = 0; i < MAX_BULLETS; i++) player->projectile[i].active = false;
    player->type = PLAYER;
    player->needs_update = true;
    player->needs_render = false;
    player->combat_update = false;
}

void init_enemies(World *world) {
    for (int i = 0, j = 0; i < NUM_ENEMIES; i++) {
        if (i < NUM_PAWNS) {
            if (i < 10) {
                world->enemies[i].position.x =
                    alien_initial_x + (HORIZONTAL_OFFSET * i);
                world->enemies[i].position.y =
                    alien_initial_y + (VERTICAL_OFFSET + (30 + 20) * 5);
            } else if (i < 20) {
                world->enemies[i].position.x =
                    alien_initial_x + (HORIZONTAL_OFFSET * (i % 10));
                world->enemies[i].position.y =
                    alien_initial_y + (VERTICAL_OFFSET + (30 + 20) * 4);

            } else {
                world->enemies[i].position.x =
                    alien_initial_x + (HORIZONTAL_OFFSET * (i % 20));
                world->enemies[i].position.y =
                    alien_initial_y + (VERTICAL_OFFSET + (30 + 20) * 3);
            }
            world->enemies[i].dimension.height = pawn_sprite.height;
            world->enemies[i].dimension.width = pawn_sprite.width;
            world->enemies[i].health.current_health = PAWN_HEALTH;
            world->enemies[i].type = PAWN;
        } else if (i < 50) {
            if (j < 10) {
                world->enemies[i].position.x =
                    alien_initial_x + (HORIZONTAL_OFFSET * (i % 30));
                world->enemies[i].position.y =
                    alien_initial_y + (VERTICAL_OFFSET + (30 + 20) * 2);
            } else {
                world->enemies[i].position.x =
                    alien_initial_x + (HORIZONTAL_OFFSET * (i % 40));
                world->enemies[i].position.y =
                    alien_initial_y + (VERTICAL_OFFSET + (30 + 20) * 1);
            }
            world->enemies[i].dimension.height = knight_sprite.height;
            world->enemies[i].dimension.width = knight_sprite.width;
            world->enemies[i].health.current_health = KNIGHT_HEALTH;
            world->enemies[i].type = KNIGHT;
            j++;
        } else if (i >= 50) {
            world->enemies[i].position.x =
                alien_initial_x + (HORIZONTAL_OFFSET * (i % 50));
            world->enemies[i].position.y = alien_initial_y + (VERTICAL_OFFSET);
            world->enemies[i].dimension.height = queen_sprite.height;
            world->enemies[i].dimension.width = queen_sprite.width;
            world->enemies[i].health.current_health = QUEEN_HEALTH;
            world->enemies[i].type = QUEEN;
        }
        world->enemies[i].needs_render = false;
        world->enemies[i].needs_update = true;
        world->enemies[i].enabled = true;
        world->enemies[i].combat_update = false;
    }

    for (int i = 0; i < 6; i++) {
        world->left_most_enemies[i] = 10 * i;
        world->right_most_enemies[i] = 10 * i + 9;
    }
}

void init_bunkers(Entity bunkers[]) {
    for (int i = 0; i < NUM_BUNKERS; i++) {
        bunkers[i].position.x = LEFT_MAX + (190 * (i + 1)) + (120 * i);
        bunkers[i].position.y = (MAP_HEIGHT)-262;
        bunkers[i].health.current_health = BUNKER_HEALTH;
        bunkers[i].dimension.height = bunker_1.height;
        bunkers[i].dimension.width = bunker_1.width;
        bunkers[i].health.current_health = BUNKER_HEALTH;
        bunkers[i].type = BUNKER;
        bunkers[i].needs_update = true;
        bunkers[i].needs_render = false;
        bunkers[i].combat_update = false;
    }
}

void move_entity(Entity *entity, Direction direction) {
    switch (direction) {
        case LEFT:
            entity->velocity.x =
                (entity->type == PLAYER) ? -PLAYER_SPEED : -HORIZONTAL_SPEED;
            entity->needs_update = true;
            break;
        case RIGHT:
            entity->velocity.x =
                (entity->type == PLAYER) ? PLAYER_SPEED : HORIZONTAL_SPEED;
            entity->needs_update = true;
            break;
        case UP:
            entity->velocity.y = -VERTICAL_SPEED;
            entity->needs_update = true;
            break;
        case DOWN:
            entity->velocity.y = VERTICAL_SPEED;
            entity->needs_update = true;
            break;
        case RESET_VERTICAL:
            entity->velocity.y = 0;
            entity->needs_update = true;
            break;
        case RESET_HORIZONTAL:
            entity->velocity.x = 0;
            entity->needs_update = true;
            break;
        default:
            entity->velocity.x = 0;
            entity->velocity.y = 0;
            entity->needs_update = true;
    }
}

Missile *create_bullet(Entity owner) {
    Missile *bullet = malloc(sizeof(Missile));
    bullet->position.x = owner.position.x + (owner.dimension.width / 2);
    bullet->position.y = owner.position.y - owner.dimension.height;
    bullet->dimension.height = red_laser.height;
    bullet->dimension.width = red_laser.width;
    bullet->needs_update = true;
    bullet->needs_render = true;
    bullet->active = true;
    return bullet;
}

void move_bullet(Missile *projectile, Direction direction) {
    switch (direction) {
        case UP:
            projectile->velocity.y = -BULLET_VELOCITY;
            break;
        case DOWN:
            projectile->velocity.y = BULLET_VELOCITY;
            break;
        default:
            projectile->velocity.y = 0;
    }
}

clock_t start = 0;

void entity_shoot(Entity *entity, Direction direction) {
    if (clock() < start) return;

    start = clock() + CLOCKS_PER_SEC / 2;

    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!entity->projectile[i].active) {
            entity->projectile[i] = *create_bullet(*entity);
            move_bullet(&entity->projectile[i], direction);
            return;
        }
    }
}

void *updateWorld(void *arg) {
    while (1) {
        update_AI_system(arg);
        update_collision_system(arg);
        update_combat_system(arg);
        update_lifebar(arg);  // ADDED
        update_movement_system(arg);
        delay(42);
    }
    return NULL;
}

void *updateRender(void *arg) {
    while (1) {
        render(arg);
    }
    return NULL;
}

void update_movement_system(World *world) {
    if (world->player.needs_update) {
        world->player.previous_pos = world->player.position;
        world->player.position.x += world->player.velocity.x;
        world->player.needs_render = true;
        world->player.needs_update = false;
    }

    for (int i = 0; i < NUM_BUNKERS; i++) {
        if (world->bunkers[i].needs_update) {
            world->bunkers[i].previous_pos = world->bunkers[i].position;
            world->bunkers[i].position.x += world->bunkers[i].velocity.x;
            world->bunkers[i].position.y += world->bunkers[i].velocity.y;
            world->bunkers[i].needs_render = true;
            world->bunkers[i].needs_update = false;
        }
    }

    for (int i = 0; i < NUM_ENEMIES; i++) {
        if (world->enemies[i].needs_update) {
            world->enemies[i].previous_pos = world->enemies[i].position;
            world->enemies[i].position.x += world->enemies[i].velocity.x;
            world->enemies[i].position.y += world->enemies[i].velocity.y;
            world->enemies[i].needs_render = true;
            world->enemies[i].needs_update = false;
        }
    }

    for (int i = 0; i < MAX_BULLETS; i++) {
        if (world->player.projectile[i].needs_update) {
            if (world->player.projectile[i].position.y > TOP_MAX) {
                world->player.projectile[i].previous_pos =
                    world->player.projectile[i].position;
                world->player.projectile[i].position.x +=
                    world->player.projectile[i].velocity.x;
                world->player.projectile[i].position.y +=
                    world->player.projectile[i].velocity.y;
                world->player.projectile[i].needs_render = true;
            } else {
                world->player.projectile[i].needs_render = false;
                world->player.projectile[i].active = false;
                world->player.projectile[i].needs_clear = true;
            }
        }
    }
}

void poll_input(World *world) {
    int *input = read_snes(gpio);

    if (!*(input + 0)) entity_shoot(&world->player, UP);
    if (!*(input + 3)) show_game_menu(world);
    if (!*(input + 6)) move_entity(&world->player, LEFT);
    if (!*(input + 7)) move_entity(&world->player, RIGHT);
    if (!*(input + 8)) entity_shoot(&world->player, UP);
    if (!*(input + 10)) move_entity(&world->player, LEFT);
    if (!*(input + 11)) move_entity(&world->player, RIGHT);

    free(input);
}

void update_AI_system(World *world) {
    /* vertical reset */
    for (int i = 0; i < NUM_ENEMIES; i++)
        move_entity(&world->enemies[i], RESET_VERTICAL);

    /* check wall collisions */
    for (int i = 0; i < 6; i++) {
        if ((world->enemies[world->right_most_enemies[i]].position.x +
             world->enemies[world->right_most_enemies[i]].dimension.width) >=
            (RIGHT_MAX)) {
            travel_right = false;
            for (int j = 0; j < NUM_ENEMIES; j++) {
                move_entity(&world->enemies[j], DOWN);
            }
        } else if ((world->enemies[world->left_most_enemies[i]].position.x) <=
                   (LEFT_MAX)) {
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
        } else {
            move_entity(&world->enemies[i], LEFT);
        }
    }
}

void update_collision_system(World *world) {
    Entity *player = &world->player;
    Entity *enemy = world->enemies;
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (player->projectile[i].active) {
            for (int j = 0; j < NUM_ENEMIES; j++)
                resolve_collisions(&player->projectile[i], &enemy[j]);
        }
    }
}

void resolve_collisions(Missile *projectile, Entity *entity) {
    bool isEnabled = entity->enabled;
    bool intersects = intersectAABB(projectile, entity);
    if (isEnabled && intersects) {
        projectile->active = false;
        projectile->needs_update = false;
        projectile->needs_render = false;
        projectile->needs_clear = true;
        entity->combat_update = true;
    }
}

bool intersectAABB(Missile *projectile, Entity *entity) {
    return projectile->position.x <
               (entity->position.x + entity->dimension.width) &&
           (projectile->position.x + projectile->dimension.width) >
               entity->position.x &&
           projectile->position.y <
               (entity->position.y + entity->dimension.height) &&
           (projectile->position.y + projectile->dimension.height) >
               entity->position.y;
}

void update_combat_system(World *world) {
    for (int i = 0; i < NUM_ENEMIES; i++) {
        if (world->enemies[i].combat_update) {
            world->enemies[i].health.current_health -= 1;
            if (world->enemies[i].health.current_health <= 0) {
                world->enemies[i].enabled = false;
                world->enemies[i].needs_clear = true;
                world->playerScore.needsRender = true;
                update_score(world, world->enemies[i].type);
            }
            world->enemies[i].combat_update = false;
        }
    }
}

void show_main_menu(Game *game) {
    drawBackground();
    drawLogo();
    drawMainMenu(game);
    while (game->main_menu.on_game_menu) {
        int *readController = read_snes(gpio);
        if (*(readController + 4) == 0) {
            game->main_menu.game_start_menu = true;
            drawMainMenu(game);
        } else if (*(readController + 5) == 0) {
            game->main_menu.game_start_menu = false;
            drawMainMenu(game);
        }

        else if (*(readController + 8) == 0) {
            game->main_menu.on_game_menu = false;
            if (game->main_menu.game_start_menu)
                game->game_start = true;
            else
                game->game_start = false;
            drawBackground();
        }
    }
}

clock_t menu_timer = 0;
void show_game_menu(World *game) {
    game->game_menu.game_menu_option = 0;
    game->game_menu.on_gameMenu_menu = true;
    while (game->game_menu.on_gameMenu_menu) {
        while (clock() < menu_timer)
            ;
        menu_timer = clock() + CLOCKS_PER_SEC / 4;

        drawGameMenu(game);
        int *readController = read_snes(gpio);
        printf("dfdfsdfsdf %d", game->game_menu.game_menu_option);
        if (*(readController + 4) == 0)  // up
        {
            if (game->game_menu.game_menu_option == 1) {
                game->game_menu.game_menu_option = 0;
            } else if (game->game_menu.game_menu_option == 2) {
                game->game_menu.game_menu_option = 1;
            }
        } else if (*(readController + 5) == 0)  // down
        {
            if (game->game_menu.game_menu_option == 0)
                game->game_menu.game_menu_option = 1;
            else if (game->game_menu.game_menu_option == 1)
                game->game_menu.game_menu_option = 2;
        } else if (*(readController + 8) == 0)  // B
        {
            if (game->game_menu.game_menu_option == 2) {
                game->game_over = true;
                printf("fdsfsdfdsdfs");
                drawBackground();
                game->game_menu.on_gameMenu_menu = false;
            }
        }
    }
}

void init_playerScore(Score *playerScore) {
    playerScore->score = 0;
    playerScore->needsUpdate = false;
    playerScore->needsRender = true;
}

void init_life(Entity *life) {
    life->health.player_health = PLAYER_HEALTH;
    life->needs_update = false;
    life->needs_render = true;
}

void update_score(World *world, Type type) {
    if (type == PAWN) world->playerScore.score += PAWN_POINTS;
    if (type == KNIGHT) world->playerScore.score += KNIGHT_POINTS;
    if (type == QUEEN) world->playerScore.score += QUEEN_POINTS;
}

void update_lifebar(World *world) {
    bool healthDecrease = true;
    if (healthDecrease == true) {
        world->life.needs_update = true;
        // world->life.health.player_health = 4;
        world->life.needs_render = true;
    }
}

