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

