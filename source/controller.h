#ifndef CONTROLLER_H
#define CONTROLLER_H

#define CLK 11
#define LAT 9
#define DAT 10
#define GPSET0 7
#define GPCLR0 10
#define GPLEV0 13

void init_gpio(unsigned int *gpio, int pin_number, int function);
void write_gpio(unsigned int *gpio, int pin_number, int n);
int read_gpio(unsigned int *gpio, int pin_number);
int read_snes(unsigned int *gpio);

#endif
