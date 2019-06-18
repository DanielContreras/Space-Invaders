#include "controller.h"

#include <stdio.h>
#include <stdlib.h>
#include "GPIO_INIT.h"
#include "controller.h"

void init_gpio(unsigned int *gpio, int pin_number, int function) {
    int reg = pin_number / 10;
    int bit = (pin_number % 10) * 3;
    gpio[reg] = (gpio[reg] & ~(0b111 << bit)) | (function << bit);
}

void write_gpio(unsigned int *gpio, int pin_number, int n) {
    if (n == 1)
        gpio[GPSET0] = 1 << pin_number;
    else
        gpio[GPCLR0] = 1 << pin_number;
}

int read_gpio(unsigned int *gpio, int pin_number) {
    return (gpio[GPLEV0] >> pin_number) & 1;
}

int *read_snes(unsigned int *gpio) {
    int *buttons = (int *)malloc(16 * sizeof(int));

    init_gpio(gpio, CLK, 0b001);
    init_gpio(gpio, LAT, 0b001);
    init_gpio(gpio, DAT, 0b000);

    write_gpio(gpio, CLK, 1);
    write_gpio(gpio, LAT, 1);
    delayMicroseconds(12);
    write_gpio(gpio, LAT, 0);

    for (int i = 0; i < 16; i++) {
        delayMicroseconds(6);
        write_gpio(gpio, CLK, 0);
        delayMicroseconds(6);
        buttons[i] = read_gpio(gpio, DAT);
        write_gpio(gpio, CLK, 1);
    }

    return buttons;
}
