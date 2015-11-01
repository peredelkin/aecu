#include "uart.h"

void uart_trx_set(uart_t* uart,uint8_t trxen) {
    uart->ucsrb.mask |= trxen;
}

void uart_mode_set(uart_t* uart,uint8_t umsel) {
    uart->ucsrc.mask |= umsel;
}

void uart_parity_set(uart_t* uart,uint8_t upm) {
    uart->ucsrc.mask |= upm;
}

void uart_stop_set(uart_t* uart,uint8_t usbs) {
    uart->ucsrc.mask |= usbs;
}

void uart_char_set(uart_t* uart,uint8_t ucsz) {
    uart->ucsrc.mask |= ucsz;
}

void uart_pol_set(uart_t* uart,uint8_t ucpol) {
    uart->ucsrc.mask |= ucpol;
}

void uart_baud_set(uart_t* uart,uint16_t baud) {
    uart->ubrr.mask |= baud;
}

void uart_setup(uart_t* uart,uint8_t trx,uint8_t umsel,uint8_t upm,uint8_t usbs,uint8_t ucsz,uint8_t ucpol,uint16_t baud) {
    uart_trx_set(uart,trx);
    uart_mode_set(uart,umsel);
    uart_parity_set(uart,upm);
    uart_stop_set(uart,usbs);
    uart_char_set(uart,ucsz);
    uart_parity_set(uart,ucpol);
    uart_baud_set(uart,baud);
}

void uart_init(uart_t* uart) {
    *uart->ubrr.reg = uart->ubrr.mask;
    *uart->ucsra.reg = uart->ucsra.mask;
    *uart->ucsrb.reg = uart->ucsrb.mask;
    *uart->ucsrc.reg = uart->ucsrc.mask;
}

void uart_int_enable(uart_int_ctrl_t* int_ctrl) {
    if (int_ctrl->flag == false) {
        *int_ctrl->uart->ucsra.reg |= int_ctrl->mask;
        *int_ctrl->uart->ucsrb.reg |= int_ctrl->mask;
        int_ctrl->flag = true;
    }
}

void uart_int_disable(uart_int_ctrl_t* int_ctrl) {
    if (int_ctrl->flag == true) {
        *int_ctrl->uart->ucsrb.reg &= ~int_ctrl->mask;
        int_ctrl->flag = false;
    }
}
