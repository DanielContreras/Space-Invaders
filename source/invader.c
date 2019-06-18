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
    restartGame = false;
    pauseGame = false;
    quitGame = false;
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
    world->enemies_alive = NUM_ENEMIES;
}

void restart_game(Game *world) {
    init_map(&world->world);
    world->game_over = false;
    world->game_start = false;
    world->main_menu.on_game_menu = true;
    world->main_menu.game_start_menu = true;
    world->game_win = false;
    restartGame = false;
    pauseGame = false;
    quitGame = false;
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
    player->needs_clear = false;
    player->combat_update = false;
    player->enabled = true;
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
        for (int j = 0; j < MAX_BULLETS; j++)
            world->enemies[i].projectile[j].active = false;
    }

    for (int i = 0; i < 6; i++) {
        world->left_most_enemies[i] = 10 * i;
        world->right_most_enemies[i] = 10 * i + 9;
    }

    for (int i = 0; i < MAX_SHOOTERS; i++) {
        world->shooters[i] = i;
    }
}

void init_bunkers(Entity bunkers[]) {
    for (int i = 0; i < NUM_BUNKERS; i++) {
        bunkers[i].position.x = LEFT_MAX + (190 * (i + 1)) + (120 * i);
        bunkers[i].position.y = (MAP_HEIGHT)-262;
        bunkers[i].previous_pos = bunkers[i].position;
        bunkers[i].health.current_health = BUNKER_HEALTH;
        bunkers[i].dimension.height = bunker_1.height;
        bunkers[i].dimension.width = bunker_1.width;
        bunkers[i].health.current_health = BUNKER_HEALTH;
        bunkers[i].type = BUNKER;
        bunkers[i].enabled = true;
        bunkers[i].needs_update = true;
        bunkers[i].needs_render = true;
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
    if (owner.type == PLAYER)
        bullet->position.y = owner.position.y - owner.dimension.height;
    else
        bullet->position.y = owner.position.y + owner.dimension.height;
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

void entity_shoot(Entity *entity, Direction direction) {
    if (clock() < entity->timer) return;

    entity->timer = clock() + CLOCKS_PER_SEC / 2;

    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!entity->projectile[i].active) {
            entity->projectile[i] = *create_bullet(*entity);
            move_bullet(&entity->projectile[i], direction);
            return;
        }
    }
}

void *updateWorld(void *arg) {
    while (!quitGame && !restartGame) {
        while(!pauseGame)
        {
            update_AI_system(arg);
            update_collision_system(arg);
            update_combat_system(arg);
            update_movement_system(arg);
            delay(42);
        }
    }
    return NULL;
}

void *updateRender(void *arg) {
    while (!quitGame && !restartGame) {
        while(!pauseGame)
        {
            render(arg);
        }
    }
    drawBackground();
    return NULL;
}

void *updateAI(void *arg) {
    while (!quitGame && !restartGame) {
        enemy_shoot(arg);
        delay(42);
    }
    return NULL;
}

void *updateInput(void *arg) {
    while (1) {
        poll_input(arg);
    }
}

void update_movement_system(World *world) {
    if (world->player.needs_update) {
        // if (!at_left_bound(world->player)) {
        //    world->player.previous_pos = world->player.position;
        //    world->player.position.x += 1;
        //} else if (!at_right_bound(world->player)) {
        //    world->player.previous_pos = world->player.position;
        //    world->player.position.x -= 1;
        //} else {
        //    world->player.previous_pos = world->player.position;
        //    world->player.position.x += world->player.velocity.x;
        //}
        world->player.previous_pos = world->player.position;
        world->player.position.x += world->player.velocity.x;
        world->player.needs_render = true;
        world->player.needs_update = false;
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

    for (int index = 0; index < 10; index++) {
        int i = world->shooters[index];
        for (int j = 0; j < MAX_BULLETS; j++) {
            if (world->enemies[i].projectile[j].needs_update) {
                if (world->enemies[i].projectile[j].position.y < BOTTOM_MAX) {
                    world->enemies[i].projectile[j].previous_pos =
                        world->enemies[i].projectile[j].position;
                    world->enemies[i].projectile[j].position.x +=
                        world->enemies[i].projectile[j].velocity.x;
                    world->enemies[i].projectile[j].position.y +=
                        world->enemies[i].projectile[j].velocity.y;
                    world->enemies[i].projectile[j].needs_render = true;
                } else {
                    world->enemies[i].projectile[j].needs_render = false;
                    world->enemies[i].projectile[j].active = false;
                    world->enemies[i].projectile[j].needs_clear = true;
                }
            }
        }
    }
}

void poll_input(World *world) {
    int *input = read_snes(gpio);

    if (!*(input + 0)) entity_shoot(&world->player, UP);
    if (!*(input + 3)) show_game_menu(&world);
    if (!*(input + 6)) move_entity(&world->player, LEFT);
    if (!*(input + 7)) move_entity(&world->player, RIGHT);
    if (!*(input + 8)) entity_shoot(&world->player, UP);
    if (!*(input + 10)) move_entity(&world->player, LEFT);
    if (!*(input + 11)) move_entity(&world->player, RIGHT);

    free(input);
}

bool enemies_at_bottom(World *world) {
    int bottom_most = 0;
    for (int i = 0; i < 10; i++) {
        if (world->enemies[bottom_most].position.y <
            world->enemies[i].position.y) {
            bottom_most = world->shooters[i];
        }
    }
    return (world->enemies[bottom_most].position.y +
            world->enemies[bottom_most].dimension.height) >
           ENEMIES_VERTICAL_MAX;
}

bool enemies_at_right_bound(World *world) {
    int right_most = 0;
    for (int i = 0; i < 6; i++) {
        if (world->enemies[world->right_most_enemies[i]].position.x >
            world->enemies[world->right_most_enemies[right_most]].position.x) {
            right_most = world->right_most_enemies[i];
        }
    }
    return (world->enemies[world->right_most_enemies[right_most]].position.x +
            world->enemies[world->right_most_enemies[right_most]]
                .dimension.width) > RIGHT_MAX;
}

bool at_vertical_bound(Entity entity) {
    return ((entity.position.x <= LEFT_MAX ||
             entity.position.x + entity.dimension.width >= RIGHT_MAX));
}

bool at_left_bound(Entity entity) { return (entity.position.x <= LEFT_MAX); }

bool at_right_bound(Entity entity) {
    return entity.position.x + entity.dimension.width >= RIGHT_MAX;
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
            if (!enemies_at_bottom(world)) {
                for (int j = 0; j < NUM_ENEMIES; j++) {
                    move_entity(&world->enemies[j], DOWN);
                }
            }
        } else if ((world->enemies[world->left_most_enemies[i]].position.x) <=
                   (LEFT_MAX)) {
            travel_right = true;
            if (!enemies_at_bottom(world)) {
                for (int j = 0; j < NUM_ENEMIES; j++) {
                    move_entity(&world->enemies[j], DOWN);
                }
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

clock_t before = 0;
void enemy_shoot(World *world) {
    if (clock() < before) return;
    before = clock() + CLOCKS_PER_SEC / 2;

    int random = rand() % 10;
    entity_shoot(&world->enemies[world->shooters[random]], DOWN);
}

void update_collision_system(World *world) {
    Entity *player = &world->player;
    Entity *enemy = world->enemies;
    Entity *bunker = world->bunkers;

    for (int i = 0; i < MAX_BULLETS; i++) {
        if (player->projectile[i].active) {
            for (int j = 0; j < NUM_ENEMIES; j++)
                resolve_collisions(&player->projectile[i], &enemy[j]);
            for (int k = 0; k < NUM_BUNKERS; k++)
                resolve_collisions(&player->projectile[i], &bunker[k]);
        }
    }

    for (int i = 0; i < 10; i++) {
        int index = world->shooters[i];
        for (int j = 0; j < MAX_BULLETS; j++) {
            if (enemy[index].projectile[j].active) {
                resolve_collisions(&enemy[index].projectile[j], player);
                for (int k = 0; k < NUM_BUNKERS; k++)
                    resolve_collisions(&enemy[index].projectile[j], &bunker[k]);
            }
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
                update_shooters(world, i);
                update_left_most(world, i);
                update_right_most(world, i);
                world->enemies_alive -= 1;
            }
            world->enemies[i].combat_update = false;
        }
    }

    for (int i = 0; i < NUM_BUNKERS; i++) {
        if (world->bunkers[i].combat_update) {
            world->bunkers[i].health.current_health -= 1;
            if (world->bunkers[i].health.current_health <= 0) {
                world->bunkers[i].enabled = false;
                world->bunkers[i].needs_clear = true;
            }
            world->bunkers[i].combat_update = false;
        }
    }

    if (world->player.combat_update) {
        world->life.needs_render = true;
        world->player.health.current_health -= 1;
        if (world->player.health.current_health <= 0) {
            world->player.enabled = false;
            world->player.needs_clear = true;
        }
        world->player.combat_update = false;
    }
}

void update_shooters(World *world, int index) {
    for (int i = 0; i < MAX_SHOOTERS; i++) {
        if (world->shooters[i] == index) {
            world->shooters[i] += 10;
        }
    }
}

void update_left_most(World *world, int index) {
    for (int i = 0; i < 6; i++) {
        if (world->left_most_enemies[i] == index) {
            world->left_most_enemies[i] += 1;
        }
    }
}

void update_right_most(World *world, int index) {
    for (int i = 0; i < 6; i++) {
        if (world->left_most_enemies[i] == index) {
            world->left_most_enemies[i] -= 1;
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
void show_game_menu(World *world) {
    world->game_menu.game_menu_option = 0;
    world->game_menu.on_gameMenu_menu = true;
    pauseGame = true;
    while (world->game_menu.on_gameMenu_menu) {
        while (clock() < menu_timer)
            ;
        menu_timer = clock() + CLOCKS_PER_SEC / 4;

        drawGameMenu(world);
        int *readController = read_snes(gpio);
        if (*(readController + 4) == 0)  // up
        {
            if (world->game_menu.game_menu_option == 1) {
                world->game_menu.game_menu_option = 0;
            } else if (world->game_menu.game_menu_option == 2) {
                world->game_menu.game_menu_option = 1;
            }
        } else if (*(readController + 5) == 0)  // down
        {
            if (world->game_menu.game_menu_option == 0)
                world->game_menu.game_menu_option = 1;
            else if (world->game_menu.game_menu_option == 1)
                world->game_menu.game_menu_option = 2;
        } else if (*(readController + 8) == 0)  // B
        {
            if (world->game_menu.game_menu_option == 0) 
            {
                world->game_menu.on_gameMenu_menu = false;
                drawBackground();
                pauseGame = false;
        
            }
            else if(world->game_menu.game_menu_option == 1)
            {
                drawBackground();
                restartGame = true;
                return;
            }
            else if(world->game_menu.game_menu_option == 2)
            {
                drawBackground();
                quitGame = true;
                return;
            }
        }
    }
    return;
}

void EndScreen(bool won){
    pauseGame = true;
    drawBackground();
    gameEndDisplay(won);
    delay(200);
    int *readController = read_snes(gpio);
    while(!restartGame){
        int *readController = read_snes(gpio);
        for (int i = 0; i<=12; i++){
            if (*(readController + i) == 0){
                restartGame = true;
            }
        }
    }
    gameEndDisplay(won);
    return;
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
    if (world->life.needs_update) {
        world->life.needs_render = true;
    }
}

