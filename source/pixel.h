#ifndef PIXEL_H
#define PIXEL_H

#include "framebuffer.h"
#include "invader.h"

typedef struct 
{
	int color;
	int x, y;
} Pixel;

typedef struct 
{
    int x, y;
    int color;
} GraphicsContext;

struct fbs framebufferstruct;
void drawPixel(int x, int y, int color);
void updateVerLoc(Pixel *pixel);
void clearPixel(int x, int y);

void init_framebuffer();
void init_context();
void render(World *world);

//void draw(Entity enemies[]);
void draw(Entity entity);
void clear(Entity entity);
//void clear_horizontal(Entity enemies[], bool isRight);
//void clear_vertical(Entity enemies[], bool isTop);
//void clear(Entity enemies[]);
//void clear_screen();
static int quit = 0;

#endif
