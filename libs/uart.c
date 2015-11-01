#include "uart.h"

void uart_interrupt_enable(uart_int_ctl_t* uart_int) {
    if (uart_int->flag == false) {
        *uart_int->uart->ucsra |= uart_int->mask;
        *uart_int->uart->ucsrb |= uart_int->mask;
        uart_int->flag = true;
    }
}

void uart_interrupt_disable(uart_int_ctl_t* uart_int) {
    if (uart_int->flag == true) {
        *uart_int->uart->ucsrb &= ~uart_int->mask;
        uart_int->flag = false;
    }
}

void uart_init_settings(uart_t* uart,
        uint8_t umsel,
        uint8_t upm,
        uint8_t usbs,
        uint8_t ucsz,
        uint8_t ucpol,
        uint16_t ubrr,
        uint8_t trxe) {
    uart->ucsrc_mask |= (umsel | upm | usbs | ucsz | ucpol);
    uart->ubrr_mask |= (UBRR_MASK & ubrr);
    uart->ucsrb_mask |= trxe;
}

void uart_init(uart_t* uart) {
    *uart->ubrr |= uart->ubrr_mask;
    *uart->ucsra |= uart->ucsra_mask;
    *uart->ucsrb |= uart->ucsrb_mask;
    *uart->ucsrc |= uart->ucsrc_mask;
}