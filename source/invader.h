#ifndef INVADER_H
#define INVADER_H

#include <stdbool.h>
#include <stdlib.h>

#define DAMAGE 1

#define PLAYER_HEALTH 5
#define PLAYER_SPEED 15

#define NUM_PAWNS 30
#define PAWN_POINTS 5
#define PAWN_HEALTH 2

#define NUM_KNIGHTS 20
#define KNIGHT_POINTS 10
#define KNIGHT_HEALTH 4

#define NUM_QUEENS 10
#define QUEEN_POINTS 100
#define QUEEN_HEALTH 15

#define NUM_BUNKERS 3
#define BUNKER_HEALTH 24

#define alien_initial_y 162
#define alien_initial_x 675
#define HORIZONTAL_OFFSET 61
#define VERTICAL_OFFSET 61
#define LEFT_MAX 400
#define RIGHT_MAX 1520
#define TOP_MAX 162
#define BOTTOM_MAX 917
#define HORIZONTAL_SPEED 4
#define VERTICAL_SPEED 6
#define ENEMIES_VERTICAL_MAX (1080 - 262 - VERTICAL_OFFSET)

#define NUM_ENEMIES (NUM_PAWNS + NUM_KNIGHTS + NUM_QUEENS)
#define NUM_ENTITIES (NUM_ENEMIES + NUM_BUNKERS + 1)
#define MAX_SHOOTERS 10

#define MAX_BULLETS 10
#define BULLET_VELOCITY 15

#define MAP_WIDTH 1920
#define MAP_HEIGHT 1080

static bool travel_right = true;
unsigned int *gpio;
bool quitGame;
bool restartGame;
bool pauseGame;

typedef struct { float x, y; } Position;

typedef struct { float x, y; } Velocity;

typedef struct { float width, height; } Dimension;

typedef union {
    int current_health;
    int player_health;
} Health;

typedef enum { PLAYER = 1, PAWN = 2, KNIGHT = 3, QUEEN = 4, BUNKER = 5 } Type;

typedef struct {
    int score;
    bool needsUpdate;
    bool needsRender;
} Score;

typedef struct {
    Position position;
    Position previous_pos;
    Velocity velocity;
    Dimension dimension;
    bool needs_update;
    bool needs_render;
    bool needs_clear;
    bool active;
} Missile;

/* ship, alien, bunker */
typedef struct {
    Position position;
    Position previous_pos;
    Velocity velocity;
    Dimension dimension;
    Health health;
    Missile projectile[MAX_BULLETS];
    Type type;
    clock_t timer;
    bool needs_update;
    bool needs_render;
    bool needs_clear;
    bool combat_update;
    bool enabled;
} Entity;

typedef struct {
    int game_menu_option;
    bool on_gameMenu_menu;
} GameMenu;

typedef struct {
    bool game_start_menu;
    bool on_game_menu;
} MainMenu;

/* struct containing all data present on the map */
typedef struct map {
    Entity player;
    Entity bunkers[NUM_BUNKERS];
    Entity enemies[NUM_ENEMIES];
    int shooters[MAX_SHOOTERS];
    int left_most_enemies[6];
    int right_most_enemies[6];
    int enemies_alive;
    bool game_over;
    Score playerScore;
    Entity life;
    GameMenu game_menu;
} World;

typedef struct {
    World world;
    bool game_win;
    bool game_over;
    bool game_start;
    MainMenu main_menu;
} Game;

typedef enum {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    STOP,
    RESET_VERTICAL,
    RESET_HORIZONTAL
} Direction;

/**
 * Initializes the game and all its defaults states
 * and flags
 * 
 * @param   *world      A pointer to world that contains all data for
 *                      the game data
 */
void init_game(Game *world);

/**
 * Initializes all the entities on the map
 * 
 * @param   *world      A pointer to world that contains all data for
 *                      the game data
 */
void init_map(World *world);

/**
 * Initializes the player entities default struct-member values
 * 
 * @param   *player     A pointer to the player entity to initialize
 */
void init_player(Entity *player);

/**
 * Initializes the enemy entities default struct-member
 * values; Will initialize pawns, knights and queens
 * 
 * @param   *world      A pointer to world that contains all data for
 *                      the game data
 */
void init_enemies(World *world);

/**
 * Initializes the bunker entities default struct-member
 * values
 * 
 * @param   *bunkers    A pointer to the bunkers array that contains all
 *                      the data for each bunker
 */
void init_bunkers(Entity bunkers[]);

/**
 * Updates the state of the world until the game is paused or
 * has ended. This function runs on its own thread
 * 
 * @param   *arg        A pointer to a void argument; required by the
 *                      POSIX thread; passed in as a reference to the 
 *                      world
 */
void *updateWorld(void *arg);

/**
 * Updates the render state of the world. This function  
 * runs asynchronously to the updateWord thread
 * 
 * @param   *arg        A pointer to a void argument; required by the
 *                      POSIX thread; passed in as a reference to the 
 *                      world
 */
void *updateRender(void *arg);

/**
 * Updates the AI state of the world. This function runs
 * asynchrounously to updateWorld and updateRender
 * 
 * @param   *arg        A pointer to a void argument; required by the
 *                      POSIX thread; passed in as a reference to the 
 *                      world
 */
void *updateAI(void *arg);

/**
 * Updates the input state of the world. Runs asynchronously to
 * undateWorld and updateRender
 * 
 * @param   *arg        A pointer to a void argument; required by the
 *                      POSIX thread; passed in as a reference to the 
 *                      world
 */
void *updateInput(void *arg);

/**
 * Script that is run by the updateAI thread. This function forces
 * a projectile to be shot from the enemy. This function calls
 * entity_shoot to cause a projectile to be shot at a given direction
 * 
 * @see     entity_shoot
 * @param   *world      A pointer to world that contains all data for
 *                      the game data
 */
void enemy_shoot(World *world);

/**
 * Moves the specified entity, player or enemy, in a given direction
 * 
 * @param   *entity     A pointer to the entity to move
 * @param   *direction  A specified direction to move the entity
 */
void move_entity(Entity *entity, Direction direction);

/**
 * Causes an entity to shoot in a given direction
 * 
 * @param   *entity     A pointer to the entity who is shooting
 * @param   *direction  A specified direction to move the projectile
 */
void entity_shoot(Entity *entity, Direction direction);

/**
 * This function updates all entities positions such that when
 * the render update occurs, the entity moves to their new location
 * 
 * @param   *world  A pointer to world that contains all data for
 *                  the game data
 */
void update_movement_system(World *world);

/**
 * Updates the combat systems for all entities. This function 
 * appends scores, decreases lives and sets flags for clearing
 * entities from the screen
 * 
 * @param   *world  A pointer to world that contains all data for
 *                  the game data
 */
void update_combat_system(World *world);

/**
 * Updates the collisions by resolving and checking if any collisions
 * occured. If they did, the collisions are resolved
 * 
 * @param   *world  A pointer to world that contains all data for
 *                  the game data
 * @see     resolve_collisions function
 */
void update_collision_system(World *world);

/**
 * Updates the AI system. This function is responsible for moving the 
 * enemies and causing them to shoot projectiles
 * 
 * @param   *world  A pointer to world that contains all data for
 *                  the game data
 */
void update_AI_system(World *world);

/**
 * Creates a new projectile and adds it to the entity who triggered
 * a shoot
 * 
 * @param   *world  A pointer to world that contains all data for
 *                  the game data
 */
Missile *create_bullet(Entity owner);

/**
 * Moves any given projectile in a given direction
 * 
 * @param   *projectile The projectile to be moved
 * @param   direction   The direction to move the projectile
 */
void move_bullet(Missile *projectile, Direction direction);

/**
 * Returns true if an collisions are detected by the missile and
 * enemy
 * 
 * @param   *projectile The projectile that is being tested for 
 *                      collision
 * @param   *entity     The entity that is being tested against
 *                      collision
 */
bool intersectAABB(Missile *projectile, Entity *entity);

/**
 * Resolves collisions for all entities and all projectiles that
 * are involved in the collision. This function will ensure that
 * all projectiles involved in collision are disabled and are 
 * flagged for a render clear. Entities will be flagged for combat
 * updates to decrease health or disable (dead)
 * 
 * @param   *projectile The projectile to resolve collisions for
 * @param   *entity     The entity to resolve collisions for
 */
void resolve_collisions(Missile *projectile, Entity *entity);

/**
 * Polls inputs from the SNES controller
 * 
 * @param   *world  A pointer to world that contains all data for
 *                  the game data
 */
void poll_input(World *world);

/**
 * Shows the main menu when called. The game is not started until
 * one start game button is clicked or the quit button is clicked.
 * Once an option is selected, the main menu will disappear and
 * trigger the game to begin
 * 
 * @param   *world  A pointer to the game that contains the flags 
 *                  for game state
 */
void show_main_menu(Game *world);

/**
 * Shows the game menu. This menu is called when the start button is
 * clicked and will pause the current game. The player may resume the
 * current game, restart the game or quit
 * 
 * @param   *world  A pointer to world that contains all data for
 *                  the game data
 */
void show_game_menu(World *game);

/**
 * Initializes the a players score to the default values (0) and
 * the flags for update and render to their default values
 * 
 * @param   *playerScore    A pointer to the struct that contains
 *                          the data for player score
 */
void init_playerScore(Score *playerScore);

/**
 * Initializes the life struct that contains data relevant for the 
 * life bar
 * 
 * @param   *life       A pointer to the life struct
 */
void init_life(Entity *life);

/**
 * Updates the player score depending on the enemy type that was 
 * killed
 * 
 * @param   *world  A pointer to world that contains all data for
 *                  the game data
 * @param   type    Type of the enemy that was killed
 */
void update_score(World *world, Type type);

/**
 * Updates an array that contains the left most entities; used for
 * wall detection of all enemies
 * 
 * @param   *world  A pointer to world that contains all data for
 *                  the game data
 */
void update_left_most(World *world, int index);

/**
 * Updates an array that contains the right most entities; used for
 * wall detection of all enemies
 * 
 * @param   *world  A pointer to world that contains all data for
 *                  the game data
 */
void update_right_most(World *world, int index);

/**
 * Updates an array that contains the enemies of each respective row
 * that contains the enemy that is the lowest; used for AI shooting
 * 
 * @param   *world  A pointer to world that contains all data for
 *                  the game data
 */
void update_shooters(World *world, int index);

/**
 * Checks if the entity is at the right bounds of the game map 
 * 
 * @param   *world  A pointer to world that contains all data for
 *                  the game data
 */
bool at_right_bound(Entity entity);

/**
 * Checks if the entity is at the left bounds of the game map
 * 
 * @param   entity  A reference to the entity to check
 */
bool at_left_bound(Entity entity);

/**
 * Checks if the enemies have reached the bottom of their playable area,
 * if they have they will no longer move down but will continue to 
 * move left or right
 * 
 * @param   *world  A pointer to world that contains all data for
 *                  the game data
 */
bool enemies_at_bottom(World *world);

#endif  // INVADER_H
