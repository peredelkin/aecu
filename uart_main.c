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
    uart_setup(&uart0,set_TRXEN(0),set_UMSEL_ASYNC,set_UPM_DIS,set_USBS_1BIT,set_UCSZ_8BIT(0),set_UCPOL_RF,set_UBRR_VALUE(9600));
    uart_init(&uart0);
    pin_out(&led);
    while (1) {
        _delay_ms(100);
        pin_toggle(&led);
    }
}
