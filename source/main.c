#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "invader.h"

int main()
{
    game game_environment = {};
    init_game(&game_environment);

    while (!game_environment.game_over)
    {
        update_world(&game_environment.game_map);
        render_world(&game_environment.game_map);
        delay(60);
    }

    return 0;
}
