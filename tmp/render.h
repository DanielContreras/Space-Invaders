#ifndef RENDER_H
#define RENDER_H

#include "framebuffer.h"
#include "invader.h"

#define FPS 60
#define SCORE_ORIGINX 1350
#define SCORE_ORIGINY 170
#define SHIFT 32

#define BAR_ORIGINX 1150
#define BAR_ORIGINY 180

typedef struct {
    int color;
    int x, y;
} Pixel;

struct fbs framebufferstruct;
void drawPixel(int x, int y, int color);
void clearPixel(int x, int y);

/*
 * Initializes the frame buffer
 */
void init_framebuffer();

/*
 * Primary entry to the graphics engine that renders all entities;
 * responsible for both drawing and clearing
 * 
 * @param   *world      A pointer to world that contains all data for
 *                      the game data
 */
void render(World *world);

/*
 * Draws the specified entities at their respective locations with
 * their respective dimensions (found inside the Entity struct)
 * 
 * @param   entity      A refence of the entity to be drawn to the 
 *                      framebuffer
 */
void draw(Entity entity);

/*
 * Clears the specified entities previous location
 * 
 * @param   entity      The entity to clear
 */
void clear(Entity entity);

/*
 * Clears the projectile at the specified position and with the 
 * given dimensions
 * 
 * @param   position    The position to clear
 * @param   dimension   The dimensions to clear
 */
void clear_projectile(Position position, Dimension dimension);

/*
 * Draws the projectile at the specified position and with the 
 * given dimensions
 * 
 * @param   position    The position to clear
 * @param   dimension   The dimensions to clear
 */
void draw_projectile(Type type, Position position, Dimension dimension);

/*
 * Draws the main menu
 * 
 * @param   *game       A pointer to the game where the menu will be drawn
 */
void drawMainMenu(Game *game);

/*
 * Draws the authors, used for the main menu
 */
void drawAuthors();

/*
 * Draws the logo, used for the main menu
 */
void drawLogo();

/*
 * Draws a black background the size of the playable area
 */
void drawBackground();

/*
 * Draws the game menu; called when the player presses the start button
 * 
 * @param   *world      A pointer to world that contains all data for
 *                      the game data
 */
void drawGameMenu(World *game);

/*
 * Draws the updated score
 * 
 * @param   num         score to draw
 * @param   x           x coordinate to draw the score at
 * @param   y           y coordinate to draw the score at
 */
void drawScore(int num, int x, int y);

/*
 * Clears the previous score
 * 
 * @param   num         score to draw
 * @param   x           x coordinate to clear the score at
 * @param   y           y coordinate to clear the score at
 */
void clearScore(int num, int x, int y);

/*
 * Draws the updated health
 * 
 * @param   health      health to draw
 * @param   x           x coordinate to draw the score at
 * @param   y           y coordinate to draw the score at
 */
void drawBar(int health, int x, int y, int w);

/*
 * Clears the previous health
 * 
 * @param   health      health to clear
 * @param   x           x coordinate to clear the score at
 * @param   y           y coordinate to clear the score at
 */
void clearBar(int health, int x, int y, int w);

#endif

