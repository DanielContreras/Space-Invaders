#include "render.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wiringPi.h>
#include "images.h"
#include "invader.h"

void init_framebuffer() { framebufferstruct = initFbInfo(); }

void render(World *world) {
    clock_t before = clock() + CLOCKS_PER_SEC / FPS;

    while (before > clock())
        ;

    for (int i = 0; i < NUM_ENEMIES; i++) {
        if (world->enemies[i].needs_render && world->enemies[i].enabled) {
            clear(world->enemies[i]);
            draw(world->enemies[i]);
            world->enemies[i].needs_render = false;
        } else if (world->enemies[i].needs_clear) {
            clear(world->enemies[i]);
            world->enemies[i].needs_clear = false;
        }
    }

    if (world->player.needs_render) {
        clear(world->player);
        draw(world->player);
        world->player.needs_render = false;
    }

    for (int i = 0; i < NUM_BUNKERS; i++) {
        if (world->bunkers[i].needs_render) {
            clear(world->bunkers[i]);
            draw(world->bunkers[i]);
        }
    }

    for (int i = 0; i < MAX_BULLETS; i++) {
        if (world->player.projectile[i].needs_render) {
            clear_projectile(world->player.projectile[i].previous_pos,
                             world->player.projectile[i].dimension);
            draw_projectile(world->player.projectile[i].position,
                            world->player.projectile[i].dimension);
        } else if (world->player.projectile[i].needs_clear) {
            clear_projectile(world->player.projectile[i].position,
                             world->player.projectile[i].dimension);
            world->player.projectile[i].needs_clear = false;
        }
    }
}

void clear_projectile(Position position, Dimension dimension) {
    int width = dimension.width;
    int height = dimension.height;

    int x = position.x;
    int oldX = x;
    int y = position.y;

    for (int i = 0; i < (width * height); i++) {
        x++;
        if (i % width == 0) {
            y++;
            x = oldX;
        }
        clearPixel(x, y);
    }
}

void draw_projectile(Position position, Dimension dimension) {
    int *colorptr;
    int width = dimension.width;
    int height = dimension.height;

    colorptr = (int *)red_laser.image_pixels;

    int x = position.x;
    int oldX = x;
    int y = position.y;

    for (int i = 0; i < (width * height); i++) {
        x++;
        if (i % width == 0) {
            y++;
            x = oldX;
        }
        drawPixel(x, y, colorptr[i]);
    }
}

void drawPixel(int x, int y, int color) {
    int offset = ((y * framebufferstruct.x) + x);
    framebufferstruct.fptr[offset] = color;
}

void clearPixel(int x, int y) {
    int offset = (((y)*framebufferstruct.x) + x);
    framebufferstruct.fptr[offset] = (int)0;
}

void draw(Entity entity) {
    int *colorptr;
    int width = entity.dimension.width;
    int height = entity.dimension.height;

    if (entity.type == PAWN)
        colorptr = (int *)pawn_sprite.image_pixels;
    else if (entity.type == KNIGHT)
        colorptr = (int *)knight_sprite.image_pixels;
    else if (entity.type == QUEEN)
        colorptr = (int *)queen_sprite.image_pixels;
    else if (entity.type == PLAYER)
        colorptr = (int *)blue_ship_sprite.image_pixels;
    else if (entity.type == BUNKER)
        colorptr = (int *)bunker_1.image_pixels;

    int x = entity.position.x;
    int oldX = x;
    int y = entity.position.y;

    for (int i = 0; i < (width * height); i++) {
        x++;
        if (i % width == 0) {
            y++;
            x = oldX;
        }
        drawPixel(x, y, colorptr[i]);
    }
}

void clear(Entity entity) {
    int width = entity.dimension.width;
    int height = entity.dimension.height;

    int x = entity.previous_pos.x;
    int oldX = x;
    int y = entity.previous_pos.y;

    for (int i = 0; i < (width * height); i++) {
        x++;
        if (i % width == 0) {
            y++;
            x = oldX;
        }
        clearPixel(x, y);
    }
}

void drawMainMenu(Game *game) {
    int *colorptrMenu;
    int widthMenu = main_menu_quit.width;
    int heightMenu = main_menu_quit.height;

    if ((game->main_menu.game_start_menu))
        colorptrMenu = (int *)main_menu_start.image_pixels;
    else
        colorptrMenu = (int *)main_menu_quit.image_pixels;

    int xMenu = (int)((MAP_WIDTH / 2) - (widthMenu / 2));
    int yMenu = TOP_MAX + LOGO.height;

    for (int i = 0; i < (widthMenu * heightMenu); i++) {
        xMenu++;
        if (i % widthMenu == 0) {
            yMenu++;
            xMenu = (int)((MAP_WIDTH / 2) - (widthMenu / 2));
        }
        drawPixel(xMenu, yMenu, colorptrMenu[i]);
    }
    drawAuthors();
}

void drawAuthors() {
    int *colorptrNames;
    int widthNames = authors.width;
    int heightNames = authors.height;
    colorptrNames = (int *)authors.image_pixels;

    int xMenu = (int)((MAP_WIDTH / 2) - (widthNames / 2));
    int yMenu = TOP_MAX + LOGO.height + main_menu_quit.height;

    for (int i = 0; i < (widthNames * heightNames); i++) {
        xMenu++;
        if (i % widthNames == 0) {
            yMenu++;
            xMenu = (int)((MAP_WIDTH / 2) - (widthNames / 2));
        }
        drawPixel(xMenu, yMenu, colorptrNames[i]);
    }
}

void drawLogo() {
    int *colorptrLogo = (int *)LOGO.image_pixels;
    int widthLogo = LOGO.width;
    int heightLogo = LOGO.height;

    int xLogo = (int)((MAP_WIDTH / 2) - (widthLogo / 2));
    int yLogo = TOP_MAX;

    for (int i = 0; i < (widthLogo * heightLogo); i++) {
        xLogo++;
        if (i % widthLogo == 0) {
            yLogo++;
            xLogo = (int)((MAP_WIDTH / 2) - (widthLogo / 2));
        }
        drawPixel(xLogo, yLogo, colorptrLogo[i]);
    }
}

void drawBackground() {
    int width = RIGHT_MAX - LEFT_MAX;
    int height = BOTTOM_MAX - TOP_MAX + 60;
    int x = LEFT_MAX;
    int y = TOP_MAX;
    for (int i = 0; i < (width * height); i++) {
        x++;
        if (i % width == 0) {
            y++;
            x = LEFT_MAX;
        }
        clearPixel(x, y);
    }
}

void drawGameMenu(World *game) {
    int *colorptrMenu;
    int widthMenu = game_menu_pause.width;
    int heightMenu = game_menu_pause.height;
    if ((game->game_menu.game_menu_option == 0))
        colorptrMenu = (int *)game_menu_pause.image_pixels;
    else if ((game->game_menu.game_menu_option == 1))
        colorptrMenu = (int *)game_menu_restart.image_pixels;
    else
        colorptrMenu = (int *)game_menu_quit.image_pixels;

    int xMenu = (int)((MAP_WIDTH / 2) - (widthMenu / 2));
    int yMenu = TOP_MAX;

    for (int i = 0; i < (widthMenu * heightMenu); i++) {
        xMenu++;
        if (i % widthMenu == 0) {
            yMenu++;
            xMenu = (int)((MAP_WIDTH / 2) - (widthMenu / 2));
        }
        drawPixel(xMenu, yMenu, colorptrMenu[i]);
    }
}
