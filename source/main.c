#include <stdio.h>
#include <time.h>
#include <wiringPi.h>
#include <ECS/ecs.h>
#include <stdio.h>

#define FPS 60
#define DELAY (1000 / FPS)

int frames, seconds;
ECS* ecs = NULL;

double ClockToMilliseconds(clock_t ticks);
void LogFPS();

int main() {
    clock_t start;
    int end_time;
    frames = 0;
    seconds = millis();

	ecs = InitializeECS();

    while (1) {
        start = clock();
        UpdateECS(ecs->engine);
        end_time = clock() - start;
        frames++;
        LogFPS();
        if ((int)DELAY > end_time) delay(DELAY - end_time);
    }
	
	TerminateECS(ecs);
}

double ClockToMilliseconds(clock_t ticks) { return (ticks / (double)CLOCKS_PER_SEC) * 1000.0; }

void LogFPS() {
    if ((long)millis() > seconds + 1000) {
        printf("Fps: %d\n", frames);
        frames = 0;
        seconds = millis();
    }
}

