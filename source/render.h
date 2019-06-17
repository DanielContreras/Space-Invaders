#ifndef RENDER_H
#define RENDER_H

#include "framebuffer.h"
#include "invader.h"

#define FPS 60

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

void clear_projectile(Position position, Dimension dimension);
void draw_projectile(Position position, Dimension dimension);

void drawMainMenu(Game *game);
void drawAuthors();
void drawLogo();
void drawBackground();
void drawGameMenu(World *game);

void drawScore(int num, int value1, int value2);
void clearScore(int entity, int value1, int value2);
void drawBar(int health, int x, int y, int w);
void clearBar(int health, int x, int y, int w);

#endif

