#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "controller.h"
#include "framebuffer.h"
#include "invader.h"

int main() 
{
	init_map();
	return 0;
}
