#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include "invader.h"

/* polls for input */
void *getInput(void* args){
    while (!quitGame) {
        poll_input(&((Game*)args)->world);
    }
}

int main() {
    Game game;
    init_game(&game);
    show_main_menu(&game);
    if(game.game_start){
        pthread_t animation_t, logic_t, input_t, ai_t;
        pthread_create(&input_t, NULL, getInput, &game);
        while (!quitGame) {
            restart_game(&game);
            pthread_t animation_t, logic_t, input_t;            
            pthread_create(&logic_t, NULL, updateWorld, &game);     
            pthread_create(&ai_t, NULL, updateAI, &game);
            pthread_create(&animation_t, NULL, updateRender, &game);
            
            pthread_join(logic_t, NULL);
            pthread_join(animation_t, NULL);
        }
        pthread_join(ai_t, NULL);
        pthread_join(input_t, NULL);
    }
    return 0;
}
