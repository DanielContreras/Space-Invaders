#include "controller.h"

#include <unistd.h>
#include <stdio.h>
#include <wiringPi.h>
#include "GPIO_INIT.h"
#include "controller.h"

/**
 * Initializes a specified GPIO line to the specified function. 
 * 
 * @param *gpio 	A pointer to an int that contains the address of the GPIO base
 * 			base address
 * @param pin_number	The specified pin number to set a fuction to
 * @param function	The specified function to set
 */
void init_gpio(unsigned int *gpio, int pin_number, int function) 
{
    int reg = pin_number / 10; 
    int bit = (pin_number % 10) * 3; 
    gpio[reg] = (gpio[reg] & ~(0b111 << bit)) | (function << bit);
}

/**
 * Writes a value (0 or 1) to the specified GPIO line (pin number) 
 * 
 * @param *gpio 	A pointer to an int that contains the address of the GPIO base
 * 			base address		
 * @param pin_number	The specified pin number that you are writing to
 * @param n		The value that you are writing to the pin number (0 or 1)
 */
void write_gpio(unsigned int *gpio, int pin_number, int n)
{
    if (n == 1)
        gpio[GPSET0] = 1 << pin_number;
    else
        gpio[GPCLR0] = 1 << pin_number;
}

/**
 * Returns the value in the specified GPIO line (pin number) 
 * 
 * @param *gpio 	A pointer to an int that contains the address of the GPIO base
 * 			base address	
 * @param  pin_number	The GPIO line (pin number) being read from
 * @return 		The current value that is found in the specified GPIO line (pin number)
 */
int read_gpio(unsigned int *gpio, int pin_number)
{
    return (gpio[GPLEV0] >> pin_number) & 1; 
}

/**
 * Returns the integer representation of the button that is pressed. If no input is given 
 * (no button press) then the function will return 0
 * 
 * @param *gpio 	A pointer to an int that contains the address of the GPIO base
 * 			base address
 * @return		An integer between 0 and 12 that represents the button that has been 
 * 			pressed
 */
int read_snes(unsigned int *gpio)
{
    int buttons[16];
    
    init_gpio(gpio, CLK, 0b001);
    init_gpio(gpio, LAT, 0b001);
    init_gpio(gpio, DAT, 0b000);
    
    write_gpio(gpio, CLK, 1);
    write_gpio(gpio, LAT, 1);
    delayMicroseconds(12);
    write_gpio(gpio, LAT, 0);

    for (int i = 0; i < 16; i++)
    {
	    delayMicroseconds(6);
	    write_gpio(gpio, CLK, 0);
	    delayMicroseconds(6);
	    buttons[i] = read_gpio(gpio, DAT);
	    write_gpio(gpio, CLK, 1);
    }
    
    for (int i = 0; i < 12; i++)
    {
	    if (buttons[i] == 0) return ++i;
    }
    return 0;
}
