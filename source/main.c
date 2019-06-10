#include <unistd.h>
#include <stdio.h>
#include <wiringPi.h>
#include "GPIO_INIT.h"
#include "controller.h"

int main()
{
    unsigned int *gpio = gpioPtr();     // Get GPIO base address
    unsigned int input = 0;
    unsigned int oldInput = 0;
    printf("Created by: Tariq Rahmani, Daniel Contreras, Benjamin Sterling, Abdelraheem Ghebari\n");
    printf("Please press a button...\n");
    // while start is not pressed
    while (oldInput != 4)
    {   
        // while no input recieved
        while(input == 0)
        {   
            input = read_snes(gpio);
            // if previous input has button value from (1-12) and new input has a 0 (button release)
            if (input == 0 && oldInput != 0)
            {   
                switch(oldInput)
                {
                    case 1:
                        printf ("You have pressed B\n");
                        break;
                    case 2:
                        printf ("You have pressed Y\n");
                        break;
                    case 3:
                        printf ("You have pressed Select\n");
                        break;
                    case 5:
                        printf ("You have pressed Joy-pad UP\n");
                        break;
                    case 6:
                        printf ("You have pressed Joy-pad DOWN\n");
                        break;
                    case 7:
                        printf ("You have pressed Joy-pad LEFT\n");
                        break;
                    case 8:
                        printf ("You have pressed Joy-pad RIGHT\n");
                        break;
                    case 9:
                        printf ("You have pressed A\n");
                        break;
                    case 10:
                        printf ("You have pressed X\n");
                        break;
                    case 11:
                        printf ("You have pressed LEFT\n");
                        break;
                    case 12:
                        printf ("You have pressed RIGHT\n");
                        break;
                }
                printf("Please press a button...\n");
                break;
            }
        }
        oldInput = input;
        input = 0;
        delay(50);
    }
    printf("Program is terminating...\n");
    return 0;
}
