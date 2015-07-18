/* 
 * File:   timers.h
 * Author: root
 *
 * Created on 16 июня 2015 г., 19:40
 */

#ifndef TIMERS_H
#define	TIMERS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef void (*timer_event) (void);

typedef struct {
    volatile uint8_t* tifr; //register of flags
    volatile uint8_t* timsk; //register of masks
} tmr16_int_reg_t;

typedef struct {
    timer_event event; //
    uint8_t mask; //mask interrupt control
    bool flag; //interrupt state
    tmr16_int_reg_t* ti_register; //interrupt control registers
} tmr16_int_ctrl_t;

#define TIMER_CONCAT_2(a,b) a ## b
#define TIMER_CONCAT_3(a,b,c) a ## b ## c
#define TIMER_TIFR(N) TIMER_CONCAT_2(TIFR,N)
#define TIMER_TIMSK(N) TIMER_CONCAT_2(TIMSK,N)
#define TIMER_ICIE(N) TIMER_CONCAT_2(ICIE,N)
#define TIMER_OCIEA(N) TIMER_CONCAT_3(OCIE,N,A)
#define TIMER_OCIEB(N) TIMER_CONCAT_3(OCIE,N,B)
#define TIMER_OCIEC(N) TIMER_CONCAT_3(OCIE,N,C)
#define TIMER_TOIE(N) TIMER_CONCAT_2(TOIE,N)

#define make_tic(N) { \
    .tifr = &TIMER_TIFR(N), \
    .timsk = &TIMER_TIMSK(N) \
}
#define make_cap(N,TIC) { \
    .event = NULL, \
    .flag = 0, \
    .mask = (1 << TIMER_ICIE(N)), \
    .ti_register = &TIC \
}
#define make_cha(N,TIC) { \
    .event = NULL, \
    .flag = 0, \
    .mask = (1 << TIMER_OCIEA(N)), \
    .ti_register = &TIC \
}
#define make_chb(N,TIC) { \
    .event = NULL, \
    .flag = 0, \
    .mask = (1 << TIMER_OCIEB(N)), \
    .ti_register = &TIC \
}
#define make_chc(N,TIC) { \
    .event = NULL, \
    .flag = 0, \
    .mask = (1 << TIMER_OCIEC(N)), \
    .ti_register = &TIC \
}
#define make_tov(N,TIC) { \
    .event = NULL, \
    .flag = 0, \
    .mask = (1 << TIMER_TOIE(N)), \
    .ti_register = &TIC \
}

#endif	/* TIMERS_H */

