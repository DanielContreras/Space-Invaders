#ifndef PIXEL_H
#define PIXEL_H

#include "framebuffer.h"

typedef struct {
	int color;
	int x, y;
} Pixel;

struct fbs framebufferstruct;
void drawPixel(Pixel *pixel);
void updateVerLoc(Pixel *pixel);
void clearPixel(Pixel *pixel);
void render();

int quit = 0;

#endif
