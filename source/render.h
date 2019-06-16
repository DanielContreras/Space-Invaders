#ifndef RENDER_H
#define RENDER_H

#include "framebuffer.h"
#include "invader.h"

typedef struct {
  int color;
  int x, y;
} Pixel;

struct fbs framebufferstruct;
void drawPixel(int x, int y, int color);
void clearPixel(int x, int y);

void init_framebuffer();
void render(World *world);

void draw(Entity entity);
void clear(Entity entity);
void renderer(Entity *entity);

void clear_projectile(Missile *projectile);
void draw_projectile(Missile *projectile);

#endif

