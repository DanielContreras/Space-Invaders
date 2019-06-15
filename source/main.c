#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <pthread.h>
#include "invader.h"

int main()
{
    Game game = {};
    init_game(&game); 
    
    pthread_t animation_t, logic_t, a_t;
    
    pthread_create(&logic_t, NULL, updateWorld, &game);
    pthread_create(&animation_t, NULL, updateRender, &game);
    pthread_join(logic_t, NULL);
   
    return 0;
}
