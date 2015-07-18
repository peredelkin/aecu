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

typedef void (*timer_event) ();

typedef struct {
    volatile uint8_t* tifr; //register of flags
    volatile uint8_t* timsk; //register of masks
} tmr16_int_reg_t;

typedef struct {
    timer_event event; //
    uint8_t mask; //mask interrupt control
    bool flag; //interrupt state
    tmr16_int_reg_t* ti_register; //interrupt control registers
    volatile uint16_t* cr;
} tmr16_int_ctrl_t;

typedef struct {
    volatile uint8_t* tccra;
    volatile uint8_t* tccrb;
    volatile uint8_t* tccrc;
} tmr16_ctrl_reg_t;

typedef struct {
    tmr16_ctrl_reg_t* ctrl_reg;
    uint8_t mask;
} tmr16_ctrl_mask_t;

#define TIMER_CONCAT_2(a,b) a ## b
#define TIMER_CONCAT_3(a,b,c) a ## b ## c
#define TIMER_TIFR(N) TIMER_CONCAT_2(TIFR,N)
#define TIMER_TIMSK(N) TIMER_CONCAT_2(TIMSK,N)
#define TIMER_ICIE(N) TIMER_CONCAT_2(ICIE,N)
#define TIMER_OCIE(N_CH) TIMER_CONCAT_2(OCIE,N_CH)
#define TIMER_TOIE(N) TIMER_CONCAT_2(TOIE,N)
#define TIMER_ICR(N) TIMER_CONCAT_2(ICR,N)
#define TIMER_OCR(N_CH) TIMER_CONCAT_2(OCR,N_CH)
#define TIMER_COM_TOGGLE(N_CH) (1<<TIMER_CONCAT_3(COM,N_CH,0))
#define TIMER_COM_CLEAR(N_CH) (1<<TIMER_CONCAT_3(COM,N_CH,1))
#define TIMER_COM_SET(N_CH) (1<<TIMER_CONCAT_3(COM,N_CH,1))|(1<<TIMER_CONCAT_3(COM,N_CH,0))

#define make_tic(N) { \
    .tifr = &TIMER_TIFR(N), \
    .timsk = &TIMER_TIMSK(N) \
}
#define make_cap(N,TIR) { \
    .cr = &TIMER_ICR(N), \
    .event = NULL, \
    .flag = 0, \
    .mask = (1 << TIMER_ICIE(N)), \
    .ti_register = &TIR \
}
#define make_ch(N_CH,TIR) { \
    .cr = &TIMER_OCR(N_CH), \
    .event = NULL, \
    .flag = 0, \
    .mask = (1 << TIMER_OCIE(N_CH)), \
    .ti_register = &TIR \
}
#define make_ovf(N,TIR) { \
    .cr = NULL, \
    .event = NULL, \
    .flag = 0, \
    .mask = (1 << TIMER_TOIE(N)), \
    .ti_register = &TIR \
}

extern void tmr16_int_enable(tmr16_int_ctrl_t* tic_type);
extern void tmr16_int_disable(tmr16_int_ctrl_t* tic_type);
extern void tmr16_event_set(tmr16_int_ctrl_t* tic_type, void (*timer_event) ());
extern void tmr16_event_call(tmr16_int_ctrl_t* tic_type);
extern uint16_t tmr16_read_cr(tmr16_int_ctrl_t* tic_type);
extern void tmr16_write_cr(tmr16_int_ctrl_t* tic_type, uint16_t val);

#endif	/* TIMERS_H */

