#include "timers.h"

void tmr16_int_enable(tmr16_int_ctrl_t* tic_type) {
    if (tic_type->flag == false) {
        *tic_type->ti_register->tifr |= tic_type->mask;
        *tic_type->ti_register->timsk |= tic_type->mask;
        tic_type->flag = true;
    }
}

void tmr16_int_disable(tmr16_int_ctrl_t* tic_type) {
    if (tic_type->flag == true) {
        *tic_type->ti_register->timsk &= ~tic_type->mask;
        tic_type->flag = false;
    }
}

void tmr16_event_set(tmr16_int_ctrl_t* tic_type, void (*timer_event) ()) {
    tic_type->event = timer_event;
}

void tmr16_event_call(tmr16_int_ctrl_t* tic_type) {
    if (tic_type->event) tic_type->event();
}

uint16_t tmr16_read_cr(tmr16_int_ctrl_t* tic_type) {
    return *tic_type->cr;
}

void tmr16_write_cr(tmr16_int_ctrl_t* tic_type, uint16_t val) {
    *tic_type->cr = val;
}