#include "pixel.h"

#include <stdlib.h>
#include <stdio.h>
#include <wiringPi.h>
#include <stdbool.h>
#include <time.h>
#include "invader.h"
#include "images.h"

void init_framebuffer()
{
    framebufferstruct = initFbInfo();
}

void render(World *world)
{
    int fps = 60;
    clock_t before = clock() + CLOCKS_PER_SEC / fps;
   
    while (before > clock());
    
    for (int i = 0; i < NUM_ENEMIES; i++) {
        if (world->enemies[i].needs_render) {
	        clear(world->enemies[i]);
    	    draw(world->enemies[i]);
            world->enemies[i].needs_render = false;
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
            world->bunkers[i].needs_render = false;
        }
    }

}

void drawPixel(int x, int y, int color)
{
    int offset = ((y * framebufferstruct.x) + x);
    framebufferstruct.fptr[offset] = color;
}

/* clear a pixel */
void clearPixel(int x, int y)
{
    int offset = (((y) * framebufferstruct.x) + x);
    framebufferstruct.fptr[offset] = (int) 0;							// Use a black pixel to clear
}

void draw(Entity entity)
{
    int *colorptr;
    int width = entity.dimension.width;
    int height = entity.dimension.height;
    
    if (entity.type == PAWN) colorptr = (int*) pawn_sprite.image_pixels;
    else if (entity.type == KNIGHT) colorptr = (int*) knight_sprite.image_pixels;
    else if (entity.type == QUEEN) colorptr = (int*) queen_sprite.image_pixels;
    else if (entity.type == PLAYER) colorptr = (int*) blue_ship_sprite.image_pixels;
    else if (entity.type == BUNKER) colorptr = (int*) bunker_1.image_pixels;

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

void clear(Entity entity)
{
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

