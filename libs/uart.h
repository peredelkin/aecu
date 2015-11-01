/* 
 * File:   uart.h
 * Author: root
 *
 * Created on 1 Ноябрь 2015 г., 19:54
 */

#ifndef UART_H
#define	UART_H

#include <stdbool.h>
#include <stdint.h>
#include <avr/interrupt.h>

#define UART_CONCAT_2(a,b) a ## b
#define UART_CONCAT_3(a,b,c) a ## b ## c

#define make_UBRR(N) UART_CONCAT_2(UBRR,N)
#define make_UCSR(N,T) UART_CONCAT_3(UCSR,N,T)
#define make_UDR(N) UART_CONCAT_2(UDR,N)

#define make_uart(N) {\
    .ubrr.reg = &make_UBRR(N),\
    .ucsra.reg = &make_UCSR(N,A),\
    .ucsrb.reg = &make_UCSR(N,B),\
    .ucsrc.reg = &make_UCSR(N,C),\
    .udr_reg = &make_UDR(N)\
}

#define make_RXCIE(UART,N) {\
    .flag = false,\
    .mask = (1 << UART_CONCAT_2(RXCIE,N)),\
    .uart = UART\
}

#define make_TXCIE(UART,N) {\
    .flag = false,\
    .mask = (1 << UART_CONCAT_2(TXCIE,N)),\
    .uart = UART\
}

#define make_UDRIE(UART,N) {\
    .flag = false,\
    .mask = (1 << UART_CONCAT_2(UDRIE,N)),\
    .uart = UART\
}

typedef struct {
    volatile uint8_t *reg;
    uint8_t mask;
} uart_ctrl8_t;

typedef struct {
    volatile uint16_t *reg;
    uint16_t mask;
} uart_ctrl16_t;

typedef struct {
    volatile uint8_t *udr_reg;
    uart_ctrl16_t ubrr;
    uart_ctrl8_t ucsra;
    uart_ctrl8_t ucsrb;
    uart_ctrl8_t ucsrc;
} uart_t;

typedef struct {
    uart_t *uart;
    uint8_t mask; //mask interrupt control
    volatile bool flag; //interrupt state
} uart_int_ctrl_t;



#endif	/* UART_H */

