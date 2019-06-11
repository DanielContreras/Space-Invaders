#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
// #include <wiringPi.h>
// #include "framebuffer.h"
// #include "controller.h"
#include "invader.h"

int main()
{
    game game_environment = {};
    init_game(&game_environment);
    return 0;
}
