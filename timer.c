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
uint8_t emu_tooth = 0;

tmr16_int_reg_t tic5 = make_tic(5);

tmr16_int_ctrl_t cap5 = make_cap(5, tic5);

tmr16_int_ctrl_t cha5 = make_cha(5, tic5);

tmr16_int_ctrl_t chb5 = make_chb(5, tic5);

tmr16_int_ctrl_t chc5 = make_chc(5, tic5);

tmr16_int_ctrl_t tov5 = make_tov(5, tic5);

void timer16_interrupt_enable(tmr16_int_ctrl_t* tic_type) {
    if (tic_type->flag == false) {
        *tic_type->ti_register->tifr |= tic_type->mask;
        *tic_type->ti_register->timsk |= tic_type->mask;
        tic_type->flag = true;
    }
}

void timer16_interrupt_disable(tmr16_int_ctrl_t* tic_type) {
    if (tic_type->flag == true) {
        *tic_type->ti_register->timsk &= ~tic_type->mask;
        tic_type->flag = false;
    }
}

void timer16_event_set(tmr16_int_ctrl_t* tic_type, void* event) {
    tic_type->event = event;
}

void timer16_event_call(tmr16_int_ctrl_t* tic_type) {
    if(tic_type->event) tic_type->event();
}

int main() {
    sei();
    pin_out(&b6);
    while (1) {
        if (emu_tooth <= 57) pin_on(&b6);
        _delay_us(50);
        pin_off(&b6);
        _delay_us(50);
        if (emu_tooth <= 58) emu_tooth++;
        else emu_tooth = 0;
    }
}

