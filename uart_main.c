#include <avr/io.h>
#include <util/delay.h>
#include "libs/uart.h"
#include "libs/ports.h"

uart_t uart0 = make_uart(0);

uart_int_ctrl_t rxcie0 = make_RXCIE(&uart0,0);

uart_int_ctrl_t txcie0 = make_TXCIE(&uart0,0);

uart_int_ctrl_t udrie0 = make_UDRIE(&uart0,0);

pin_t led = make_pin(B, 7);

int main() {
    sei();
    pin_out(&led);
    while (1) {
        _delay_ms(100);
        pin_toggle(&led);
    }
}
