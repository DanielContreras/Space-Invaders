#include "render.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wiringPi.h>
#include "images.h"
#include "invader.h"

#define SCORE_ORIGINX 1600
#define SCORE_ORIGINY 20
#define SHIFT 32

#define BAR_ORIGINX 1400
#define BAR_ORIGINY 30

void clearScore1(Score score);
void drawScore1(Score score);

void init_framebuffer() { framebufferstruct = initFbInfo(); }

void render(World *world) {
    clock_t before = clock() + CLOCKS_PER_SEC / FPS;

    while (before > clock())
        ;

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
            // world->bunkers[i].needs_render = false;
        }
    }

    for (int i = 0; i < MAX_BULLETS; i++) {
        if (world->player.projectile[i].needs_render) {
            clear_projectile(&world->player.projectile[i]);
            draw_projectile(&world->player.projectile[i]);
        }
    }


    	if (world->playerScore.needsRender) {
            int ones = (world->playerScore.score % 10);
            int tens = (world->playerScore.score % 100)/10;
            int hundreds = (world->playerScore.score % 1000)/100; 
            int thousands=(world->playerScore.score % 10000)/1000;
            
            clearScore(thousands, SCORE_ORIGINX, SCORE_ORIGINY);
            clearScore(hundreds, SCORE_ORIGINX + SHIFT, SCORE_ORIGINY);
            clearScore(tens, SCORE_ORIGINX + SHIFT + SHIFT, SCORE_ORIGINY);
            clearScore(ones, SCORE_ORIGINX + SHIFT + SHIFT + SHIFT, SCORE_ORIGINY);
            
            drawScore(thousands, SCORE_ORIGINX, SCORE_ORIGINY);
            drawScore(hundreds, SCORE_ORIGINX + SHIFT, SCORE_ORIGINY);
            drawScore(tens, SCORE_ORIGINX + SHIFT + SHIFT, SCORE_ORIGINY);
            drawScore(ones, SCORE_ORIGINX + SHIFT + SHIFT + SHIFT, SCORE_ORIGINY);
            
            world -> playerScore.needsRender = true; 
	}
	
	if(world-> life.needs_render) {
	    int w;
	    //int chealth = 3;
	    int chealth = (world->life.health.player_health);
	    if (chealth == 5) {w = 150;}
	    else if (chealth == 4) {w = 120;}
	    else if (chealth == 3) {w = 90;}
	    else if (chealth == 2) {w = 60;}
	    else if (chealth == 1) {w = 30;}
	    else if (chealth == 1) {w = 0;}
	    clearBar(chealth, BAR_ORIGINX, BAR_ORIGINY, w); 
	    drawBar(chealth, BAR_ORIGINX, BAR_ORIGINY, w);
	    world -> life.needs_render = false;
	    
	}
}

void clear_projectile(Missile *projectile) {
    int width = projectile->dimension.width;
    int height = projectile->dimension.height;

    int x = projectile->previous_pos.x;
    int oldX = x;
    int y = projectile->previous_pos.y;

    for (int i = 0; i < (width * height); i++) {
        x++;
        if (i % width == 0) {
            y++;
            x = oldX;
        }
        clearPixel(x, y);
    }
}

void draw_projectile(Missile *projectile) {
    int *colorptr;
    int width = projectile->dimension.width;
    int height = projectile->dimension.height;

    colorptr = (int *)red_laser.image_pixels;

    int x = projectile->position.x;
    int oldX = x;
    int y = projectile->position.y;

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

void drawScore(int num, int value1, int value2)
{
    int *colorptr; 
    int width = 32;
    int height = 51;
    
    
    if (num == 1) colorptr = (int*) ONE_IMAGE.image_pixels;
    else if (num == 2) colorptr = (int*) TWO_IMAGE.image_pixels;
    else if (num == 3) colorptr = (int*) THREE_IMAGE.image_pixels;
    else if (num == 4) colorptr = (int*) FOUR_IMAGE.image_pixels;
    else if (num == 5) colorptr = (int*) FIVE_IMAGE.image_pixels;
    else if (num == 6) colorptr = (int*) SIX_IMAGE.image_pixels;
    else if (num == 7) colorptr = (int*) SEVEN_IMAGE.image_pixels;
    else if (num == 8) colorptr = (int*) EIGHT_IMAGE.image_pixels;
    else if (num == 9) colorptr = (int*) NINE_IMAGE.image_pixels;
    else if (num == 0) colorptr = (int*) ZERO_IMAGE.image_pixels;
  
  
 
    int x = value1;
    int y = value2;
    int oldX = x;
   
    
    for (int i = 0; i < (width * height); i++) {
	    x++;
	    if (i % width == 0) {
	        y++;
	        x = oldX;
	    }
	    drawPixel(x, y, colorptr[i]);
    }
    

}

void clearScore(int entity, int value1, int value2)
{
    int width = 32;
    int height = 51;
    
    int x = value1;
    int y = value2;
    int oldX = x;
    
    for (int i = 0; i < (width * height); i++) {
	    x++;
	    if (i % width == 0) {
	        y++;
	        x = oldX;
	    }
	    clearPixel(x, y);
    }
}

void drawBar(int health, int x, int y, int w)
{
    int *colorptr;
    int width = w;
    int height = 33;
    
    
    if (health == 5) colorptr = (int*) LIFE100.image_pixels;
    else if (health == 4) colorptr = (int*) LIFE80.image_pixels;
    else if (health == 3) colorptr = (int*) LIFE60.image_pixels;
    else if (health == 2) colorptr = (int*) LIFE40.image_pixels;
    else if (health == 1) colorptr = (int*) LIFE20.image_pixels;
    


    int oldX = x;
   
    
    for (int i = 0; i < (width * height); i++) {
	    x++;
	    if (i % width == 0) {
	        y++;
	        x = oldX;
	    }
	    drawPixel(x, y, colorptr[i]);
    }
    
    
    
}


void clearBar(int health, int x, int y, int w)
{
    int width = w;
    int height = 33;
    
    int oldX = x;
    
    for (int i = 0; i < (width * height); i++) {
	    x++;
	    if (i % width == 0) {
	        y++;
	        x = oldX;
	    }
	    clearPixel(x, y);
    }
}
