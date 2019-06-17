#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include "invader.h"

int main() {
    Game game;
    init_game(&game);
    show_main_menu(&game);

    if (game.game_start) {
        pthread_t animation_t, logic_t, ai_t, input_t;
        pthread_create(&logic_t, NULL, updateWorld, &game);
        pthread_create(&ai_t, NULL, updateAI, &game);
        pthread_create(&animation_t, NULL, updateRender, &game.world);
        pthread_create(&input_t, NULL, updateInput, &game.world);
        // while (1) {
        //    poll_input(&game.world);
        //}
        pthread_join(logic_t, NULL);
        pthread_join(ai_t, NULL);
        pthread_join(animation_t, NULL);
    }

    return 0;
}

