#include "invader.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>

int main() {
  Game game;
  init_game(&game);

  pthread_t animation_t, logic_t;
  pthread_create(&logic_t, NULL, updateWorld, &game);
  pthread_create(&animation_t, NULL, updateRender, &game);

  while (1) {
    poll_input(&game.world);
  }

  pthread_join(logic_t, NULL);

  return 0;
}
