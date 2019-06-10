#ifndef CONTROLLER_H
#define CONTROLLER_H

#define CLK 11		// GPIO line for the clock
#define LAT 9		// GPIO line for the latch
#define DAT 10		// GPIO line for the data line
#define GPSET0 7	// GPSET0 (set) register 
#define GPCLR0 10	// GPCLR0 (clear) register
#define GPLEV0 13	// GPLEV0 (read) register

void init_gpio(unsigned int *gpio, int pin_number, int function);
void write_gpio(unsigned int *gpio, int pin_number, int n);
int read_gpio(unsigned int *gpio, int pin_number);
int read_snes(unsigned int *gpio);

#endif
