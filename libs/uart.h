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

#define set_TXEN(N) (1<<UART_CONCAT_2(TXEN,N))
#define set_RXEN(N) (1<<UART_CONCAT_2(RXEN,N))
#define set_TRXEN(N) (1<<UART_CONCAT_2(TXEN,N))|(1<<UART_CONCAT_2(RXEN,N))

#define set_UMSEL_ASYNC 0
#define set_UMSEL_SYNC(N) (1<<UART_CONCAT_3(UMSEL,N,0))
#define set_UMSEL_SPI(N) (1<<UART_CONCAT_3(UMSEL,N,1))|(1<<UART_CONCAT_3(UMSEL,N,0))

#define set_UPM_DIS 0
#define set_UPM_EVEN(N) (1<<UART_CONCAT_3(UPM,N,1))
#define set_UPM_ODD(N) (1<<UART_CONCAT_3(UPM,N,1))|(1<<UART_CONCAT_3(UPM,N,0))

#define set_USBS_1BIT 0
#define set_USBS_2BIT (1<<UART_CONCAT_2(USBS,N))

#define set_UCSZ_8BIT(N) (1<<UART_CONCAT_3(UCSZ,N,1))|(1<<UART_CONCAT_3(UCSZ,N,0))

#define set_UCPOL_RF 0
#define set_UCPOL_FR(N) (1<<UART_CONCAT_2(UCPOL,N))

#define set_UBRR_VALUE(BAUD) (F_CPU / 16 / BAUD - 1)

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

extern void uart_trx_set(uart_t* uart,uint8_t trxen);
extern void uart_mode_set(uart_t* uart,uint8_t umsel);
extern void uart_parity_set(uart_t* uart,uint8_t upm);
extern void uart_stop_set(uart_t* uart,uint8_t usbs);
extern void uart_char_set(uart_t* uart,uint8_t ucsz);
extern void uart_pol_set(uart_t* uart,uint8_t ucpol);
extern void uart_baud_set(uart_t* uart,uint16_t baud);
extern void uart_setup(uart_t* uart,uint8_t trx,uint8_t umsel,uint8_t upm,uint8_t usbs,uint8_t ucsz,uint8_t ucpol,uint16_t baud);
extern void uart_init(uart_t* uart);
extern void uart_int_enable(uart_int_ctrl_t* int_ctrl);
extern void uart_int_disable(uart_int_ctrl_t* int_ctrl);

#endif	/* UART_H */

