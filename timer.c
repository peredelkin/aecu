/* 
 * File:   timer.c
 * Author: root
 *
 * Created on 16 июня 2015 г., 19:38
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "libs/timers.h"
#include "libs/ports.h"

pin_t b6 = make_pin(B, 6); //emu out
pin_t b7 = make_pin(B, 7); //test out
uint8_t emu_tooth = 0;

tmr16_int_reg_t tir5 = make_tic(5);

tmr16_int_ctrl_t cap5 = make_cap(5, tir5);

tmr16_int_ctrl_t cha5 = make_ch(5A, tir5);

tmr16_int_ctrl_t chb5 = make_ch(5B, tir5);

tmr16_int_ctrl_t chc5 = make_ch(5C, tir5);

tmr16_int_ctrl_t ovf5 = make_ovf(5, tir5);

#define TIMER_CS_1(N) (1<<TIMER_CONCAT_3(CS,N,0))
#define TIMER_CS_8(N) (1<<TIMER_CONCAT_3(CS,N,1))
#define TIMER_CS_64(N) (1<<TIMER_CONCAT_3(CS,N,1))|(1<<TIMER_CONCAT_3(CS,N,0))
#define TIMER_CS_256(N) (1<<TIMER_CONCAT_3(CS,N,2))
#define TIMER_CS_1024(N) (1<<TIMER_CONCAT_3(CS,N,2))|(1<<TIMER_CONCAT_3(CS,N,0))

tmr16_ctrl_reg_t tcr5 = {
    .tccra = &TCCR5A,
    .tccrb = &TCCR5B,
    .tccrc = &TCCR5C,
};

tmr16_ctrl_mask_t cs64 = {
    .ctrl_reg = tcr5,
    .mask = TIMER_CS_64(5)
};

void test_toggle(void) {
    pin_toggle(&b7);
}

ISR(TIMER5_OVF_vect) {
    tmr16_event_call(&ovf5);
}

int main() {
    sei();
    pin_out(&b6);
    pin_out(&b7);
    tmr16_event_set(&ovf5,test_toggle);
    tmr16_int_enable(&ovf5);
    while (1) {
        if (emu_tooth <= 57) pin_on(&b6);
        _delay_us(50);
        pin_off(&b6);
        _delay_us(50);
        if (emu_tooth <= 58) emu_tooth++;
        else emu_tooth = 0;
    }
}

