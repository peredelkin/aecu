/* 
 * File:   fan.c
 * Author: root
 *
 * Created on 27 Октябрь 2015 г., 20:50
 */

#include <avr/io.h>
#include <util/delay.h>
#include "libs/ports.h"

pin_t led = make_pin(B,7);

int main() {
    pin_out(&led);
    while(1) {
        _delay_us(10);
        pin_toggle(&led);
    }
}

