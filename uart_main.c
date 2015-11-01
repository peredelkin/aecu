#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "libs/ports.h"
#include "libs/uart.h"

uart_t uart0 = make_uart(0);
uart_int_ctl_t txcie0 = make_txcie(0, uart0);
uart_int_ctl_t rxcie0 = make_rxcie(0, uart0);
uart_int_ctl_t udrie0 = make_udrie(0, uart0);

void uart_main_init(void) {
    uart_init_settings(&uart0,UMSEL_ASYNC,UPM_DIS,USBS_1BIT,UCSZ_8BIT(0),UCPOL_RF,UBRR_VALUE(9600),UTRXE(0));
    uart_init(&uart0);
}

ISR(USART0_RX_vect) {

}

ISR(USART0_TX_vect) {

}

ISR(USART0_UDRE_vect) {

}

pin_t led = make_pin(B, 7);

int main() {
    uart_main_init();
    sei();
    pin_out(&led);
    while (1) {
        _delay_ms(100);
        pin_toggle(&led);
    }
}
