#include "pixel.h"

#include <stdlib.h>
#include "invader.h"

//void render(unsigned char *image_pixels)
void render(map *world)
{
    framebufferstruct = initFbInfo();
    int i; 
    int length = 16*16;
    Pixel *pixels[length];
    int var_y = 650;
    int var_x = 800;  

    int *colorptr = (int*) alienn.image_pixels;
    for ( i = 0; i < length; i++) 
    {
	pixels[i] = malloc(sizeof(Pixel));
	pixels[i]->x = var_x++;
	pixels[i]->y = var_y;
	
	pixels[i]->color = colorptr[i];
	drawPixel(pixels[i]);
	
	
	if (i % 16 == 0) {
		var_y++;
		var_x = 800; 
		//printf("%d\n", var_y);
		//printf("%d\n", var_x);
	}
	free(pixels[i]);
	pixels[i] = NULL;
    }  
}

void drawPixel(Pixel *pixel)
{
    int offset = ((pixel->y * framebufferstruct.x) + pixel->x);
    framebufferstruct.fptr[offset] = pixel->color;
}

/* update the verical coordinates of a pixel */
void updateVerLoc(Pixel *pixel)
{
    pixel->y -= 1;
    if (pixel->y >= framebufferstruct.y+1) quit = 1; 
}

/* clear a pixel */
void clearPixel(Pixel *pixel)
{
    int offset = (((pixel->y) * framebufferstruct.x) + pixel->x);
    framebufferstruct.fptr[offset] = (int) 0;							// Use a black pixel to clear
}
