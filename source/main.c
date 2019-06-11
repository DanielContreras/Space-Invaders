#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wiringPi.h>
// #include "framebuffer.h"
// #include "controller.h"
#include "invader.h"
//#include <time.h>

//void delay(int number_of_seconds);

int main()
{
    game game_environment = {};
    init_game(&game_environment);

    while (!game_environment.game_over)
    {
        update_world(&game_environment.game_map);
        render_world();
        delay(60);
    }

    return 0;
}

//void delay(int number_of_seconds)
//{
    // Converting time into milli_seconds
//    int milli_seconds = 1000 * number_of_seconds;

    // Stroing start time
//    clock_t start_time = clock();

    // looping till required time is not acheived
//    while (clock() < start_time + milli_seconds);
//}
