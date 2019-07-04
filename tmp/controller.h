#ifndef CONTROLLER_H
#define CONTROLLER_H

#define CLK 11
#define LAT 9
#define DAT 10
#define GPSET0 7
#define GPCLR0 10
#define GPLEV0 13

/**
 * Initializes a specified GPIO line to the specified function. 
 * 
 * @param *gpio 	    A pointer to an int that contains the address of the GPIO base
 * 			            base address
 * @param pin_number	The specified pin number to set a fuction to
 * @param function	    The specified function to set
 */
void init_gpio(unsigned int *gpio, int pin_number, int function);

/**
 * Writes a value (0 or 1) to the specified GPIO line (pin number) 
 * 
 * @param *gpio 	    A pointer to an int that contains the address of the GPIO base
 * 			            base address		
 * @param pin_number	The specified pin number that you are writing to
 * @param n		        The value that you are writing to the pin number (0 or 1)
 */
void write_gpio(unsigned int *gpio, int pin_number, int n);

/**
 * Returns the value in the specified GPIO line (pin number) 
 * 
 * @param *gpio 	    A pointer to an int that contains the address of the GPIO base
 * 			            base address	
 * @param  pin_number	The GPIO line (pin number) being read from
 * @return 		        The current value that is found in the specified GPIO line (pin number)
 */
int read_gpio(unsigned int *gpio, int pin_number);

/**
 * Returns the integer representation of the button that is pressed. If no input is given 
 * (no button press) then the function will return 0
 * 
 * @param *gpio 	A pointer to an int that contains the address of the GPIO base
 * 			        base address
 * @return		    An 16-bit integer that represents the values read from the DAT line
 */
int *read_snes(unsigned int *gpio);

#endif
