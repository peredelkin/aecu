#include "usart.h"

void uart_init(uart_t* uart) {
    *uart->ucsrb |= /*(1<<RXEN0)|*/(1 << TXEN0);
}

void uart_baud_rate(uart_t* uart, uint16_t ubrr) {
    *uart->ubrrh = (uint8_t) (ubrr >> 8);
    *uart->ubrrl = (uint8_t) (ubrr);
}

void uart_character_size(uart_t* uart, uint8_t size) {
    if (size) {
        if (size == 5) {
            *uart->ucsrb &= ~(1 << UCSZ20);
            *uart->ucsrc &= ~((1 << UCSZ10) | (1 << UCSZ00));
        } //5-bit
        if (size == 6) *uart->ucsrc |= (1 << UCSZ00); //6-bit
        if (size == 7) *uart->ucsrc |= (1 << UCSZ10); //7-bit
        if (size == 8) *uart->ucsrc |= (1 << UCSZ10) | (1 << UCSZ00); //8-bit
        if (size == 9) {
            *uart->ucsrb |= (1 << UCSZ20);
            *uart->ucsrc |= (1 << UCSZ10) | (1 << UCSZ00);
        } //9-bit
    } else *uart->ucsrc |= (1 << UCSZ10) | (1 << UCSZ00); //8-bit
}

void uart_parity_mode(uart_t* uart, uint8_t mode) {
    if (mode) {
        if (mode == 2) *uart->ucsrc |= (1 << UPM10);
        if (mode == 3) *uart->ucsrc |= (1 << UPM10) | (1 << UPM00);
    } else *uart->ucsrc &= ~((1 << UPM10) | (1 << UPM00));
}

void uart_stop_bit(uart_t* uart, uint8_t bit) {
    if (bit == 2) *uart->ucsrc |= (1 << USBS0);
    else *uart->ucsrc &= ~(1 << USBS0);
}

void uart_tx(uart_t* uart, uint8_t* data, uint8_t data_size) {
    while (*uart->ucsrb & (1 << UDRIE0));
    uart->tx_data_index = 0;
    uart->tx_data = data;
    uart->tx_data_size = data_size;
    *uart->ucsrb |= (1 << UDRIE0);
    //while(*uart->ucsrb & (1 << UDRIE));
}

void uart_udre_handler(uart_t* uart) {
    if (uart->tx_data_index < uart->tx_data_size) {
        *uart->udr = uart->tx_data[uart->tx_data_index];
        uart->tx_data_index++;
    } else {
        //запрещаем прерывание UDRE
        *uart->ucsrb &= ~(1 << UDRIE0);
    }
}

void uart_rx(uart_t* uart, uint8_t* data, uint8_t data_size) {
    while (*uart->ucsrb & (1 << RXCIE0));
    uart->rx_data_index = 0;
    uart->rx_data = data;
    uart->rx_data_size = data_size;
    *uart->ucsrb |= (1 << RXCIE0);
}

void uart_rx_handler(uart_t* uart) {
    if (uart->rx_data_index < uart->rx_data_size) {
        uart->rx_data[uart->rx_data_index] = *uart->udr;
        uart->rx_data_index++;
    } else {
        *uart->ucsrb &= ~(1 << RXCIE0);
    }
}

uint8_t uart_rx_count(uart_t* uart) {
    return uart->rx_data_index;
}

uint8_t uart_rx_done(uart_t* uart) {
    return uart->rx_data_index == uart->rx_data_size;
}

uint8_t uart_tx_done(uart_t* uart) {
    return uart->tx_data_index == uart->tx_data_size;
}