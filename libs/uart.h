/* 
 * File:   uart.h
 * Author: root
 *
 * Created on 1 Ноябрь 2015 г., 17:09
 */

#ifndef UART_H
#define	UART_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    volatile uint8_t* udr; //I/O register
    volatile uint8_t* ucsra; //control register A
    uint8_t ucsra_mask; //control mask A
    volatile uint8_t* ucsrb; //control register B
    uint8_t ucsrb_mask; //control mask B
    volatile uint8_t* ucsrc; //control register C
    uint8_t ucsrc_mask; //control mask C
    volatile uint16_t* ubrr; //baud rate register
    uint16_t ubrr_mask; //baud rate mask
} uart_t;

typedef struct {
    //event
    uart_t* uart; //uart control
    uint8_t mask; //interrupt mask
    volatile bool flag; //interrupt state
} uart_int_ctl_t;

#define UART_CONCAT_2(a,b) a ## b
#define UART_CONCAT_3(a,b,c) a ## b ## c

#define UMSEL_ASYNC 0
#define UMSEL_SYNC(N) (1<<UART_CONCAT_3(UMSEL,N,0))
#define UMSEL_SPI(N) (1<<UART_CONCAT_3(UMSEL,N,1))|(1<<UART_CONCAT_3(UMSEL,N,0))

#define UPM_DIS 0
#define UPM_EVEN(N) (1<<UART_CONCAT_3(UPM,N,1))
#define UPM_ODD(N) (1<<UART_CONCAT_3(UPM,N,1))|(1<<UART_CONCAT_3(UPM,N,0))

#define USBS_1BIT 0
#define USBS_2BIT(N) (1<<UART_CONCAT_2(USBS,N))

#define UCSZ_8BIT(N) (1<<UART_CONCAT_3(UCSZ,N,1))|(1<<UART_CONCAT_3(UCSZ,N,0))

#define UCPOL_RF 0
#define UCPOL_FR(N) (1<<UART_CONCAT_2(UCPOL,N))

#define UBRR_VALUE(UART_BAUD) (F_CPU / 16 / UART_BAUD - 1)
#define UBRR_MASK 0xFFF

#define UTXE(N) (1<<UART_CONCAT_2(TXEN,N))
#define URXE(N) (1<<UART_CONCAT_2(RXEN,n))
#define UTRXE(N) (1<<UART_CONCAT_2(TXEN,N))|(1<<UART_CONCAT_2(RXEN,N))

#define make_uart(N) {\
    .ubrr = &UART_CONCAT_2(UBRR,N),\
    .ucsra = &UART_CONCAT_3(UCSR,N,A),\
    .ucsrb = &UART_CONCAT_3(UCSR,N,B),\
    .ucsrc = &UART_CONCAT_3(UCSR,N,C),\
    .udr = &UART_CONCAT_2(UDR,N)\
}

#define make_txcie(N,UART) {\
    .flag = false,\
    .mask = (1<<UART_CONCAT_2(TXCIE,N)),\
    .uart = &UART\
}

#define make_rxcie(N,UART) {\
    .flag = false,\
    .mask = (1<<UART_CONCAT_2(RXCIE,N)),\
    .uart = &UART\
}

#define make_udrie(N,UART) {\
    .flag = false,\
    .mask = (1<<UART_CONCAT_2(UDRIE,N)),\
    .uart = &UART\
}

extern void uart_interrupt_enable(uart_int_ctl_t* uart_int);
extern void uart_interrupt_disable(uart_int_ctl_t* uart_int);
extern void uart_init_settings(uart_t* uart,
        uint8_t umsel,
        uint8_t upm,
        uint8_t usbs,
        uint8_t ucsz,
        uint8_t ucpol,
        uint16_t ubrr,
        uint8_t trxe);
extern void uart_init(uart_t* uart);

#endif	/* UART_H */

