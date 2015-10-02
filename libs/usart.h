/* 
 * File:   usart.h
 * Author: ruslan
 *
 * Created on 26 Сентябрь 2014 г., 22:44
 */

#ifndef USART_H
#define	USART_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>


//#define UART_BAUD 57600

#define UART_UBRR_VALUE (F_CPU / 16 / UART_BAUD - 1)

#define UART_CONCAT(a,b,c) a ## b ## c
#define GEN_UDR(P) UDR##P
#define GEN_UCSRA(P) UART_CONCAT(UCSR,P,A)
#define GEN_UCSRB(P) UART_CONCAT(UCSR,P,B)
#define GEN_UCSRC(P) UART_CONCAT(UCSR,P,C)
#define GEN_UBRRH(P) UART_CONCAT(UBRR,P,H)
#define GEN_UBRRL(P) UART_CONCAT(UBRR,P,L)
#define UPM_DIS 0
#define UPM_EVEN 2
#define UPM_ODD 3


typedef struct {
    volatile uint8_t* udr;
    volatile uint8_t* ucsra;
    volatile uint8_t* ucsrb;
    volatile uint8_t* ucsrc;
    volatile uint8_t* ubrrh;
    volatile uint8_t* ubrrl;
    uint8_t*    tx_data;
    uint8_t     tx_data_size;
    uint8_t     tx_data_index;
    uint8_t*    rx_data;
    uint8_t     rx_data_size;
    uint8_t     rx_data_index;
} uart_t;


#define UART_init(P) {.udr=&GEN_UDR(P), .ucsra=&GEN_UCSRA(P), .ucsrb=&GEN_UCSRB(P), .ucsrc=&GEN_UCSRC(P), .ubrrh=&GEN_UBRRH(P), .ubrrl=&GEN_UBRRL(P)} //получение адресов регистров

//uart_t uart0 = uart_init(0); //UART0
//uart_t uart1 = uart_init(1); //UART1

extern void uart_tx_init(uart_t* uart);
extern void uart_rx_init(uart_t* uart);
extern void uart_baud_rate(uart_t* uart,uint16_t ubrr);
extern void uart_character_size(uart_t* uart,uint8_t size);
extern void uart_parity_mode(uart_t* uart,uint8_t mode);
extern void uart_stop_bit(uart_t* uart,uint8_t bit);
extern void uart_tx(uart_t* uart,uint8_t* data,uint8_t data_size);
extern void uart_udre_handler(uart_t* uart);
extern void uart_rx(uart_t* uart,uint8_t* data,uint8_t data_size);
extern void uart_rx_handler(uart_t* uart);
extern uint8_t uart_rx_count(uart_t* uart);
extern uint8_t uart_rx_done(uart_t* uart);
extern uint8_t uart_tx_done(uart_t* uart);

#endif	/* USART_H */


/* Пример
#include <avr/io.h>
#include "libs/usart.h"
#include <util/delay.h>

#define UART_BAUD 57600
#define UART_UBRR_VALUE (F_CPU / 16 / UART_BAUD - 1)

uart_t uart0 = UART_init(0);
uint8_t done=0;

ISR(USART0_UDRE_vect)
{
    uart_udre_handler(&uart0);
}

ISR(USART0_RX_vect)
{
    uart_rx_handler(&uart0);
}

int main() {
    uart_init(&uart0);
    uart_baud_rate(&uart0,UART_UBRR_VALUE);
    uart_character_size(&uart0,8);
    uart_parity_mode(&uart0,UPM_DIS);
    uart_stop_bit(&uart0,1);
    sei();
    uart_tx(&uart0,"Atmega 64",9);
    char data[10];
    while(1){
        if(uart_rx_done(&uart0)){
            uart_tx(&uart0, data, 10);
            uart_rx(&uart0,data,10);
        }
    }
}
 */
