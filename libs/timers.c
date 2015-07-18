#include "timers.h"

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