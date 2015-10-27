#include <avr/io.h>
#include <util/delay.h>
#include "libs/ports.h"

pin_t led = make_pin(B,7);

int main() {
    pin_out(&led);
    while(1) {
        _delay_ms(100);
        pin_toggle(&led);
    }
}
