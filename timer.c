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
#include "libs/font.h"

pin_t b5 = make_pin(B, 5); //coil out
pin_t b6 = make_pin(B, 6); //emu out
pin_t b7 = make_pin(B, 7); //test out
pin_t l1 = make_pin(L, 1); //capture in
uint8_t emu_tooth = 0;

tmr16_int_reg_t tir4 = make_tir(4);
tmr16_int_ctrl_t cha4 = make_ch(4A, tir4);
tmr16_int_ctrl_t chb4 = make_ch(4B, tir4);
tmr16_int_ctrl_t chc4 = make_ch(4C, tir4);
tmr16_ctrl_reg_t tcr4 = make_tcr(4);
tmr16_ctrl_mask_t tcs4 = make_cs8_mask(4, tcr4);
tmr16_ctrl_mask_t tcnt4_mask = {
    .ctrl_reg = &tcr4,
    .mask = 0
};

tmr16_int_reg_t tir5 = make_tir(5);
tmr16_int_ctrl_t cap5 = make_cap(5, tir5);
tmr16_int_ctrl_t cha5 = make_ch(5A, tir5);
tmr16_int_ctrl_t chb5 = make_ch(5B, tir5);
tmr16_int_ctrl_t chc5 = make_ch(5C, tir5);
tmr16_int_ctrl_t ovf5 = make_ovf(5, tir5);
tmr16_ctrl_reg_t tcr5 = make_tcr(5);
tmr16_ctrl_mask_t tcs5 = make_cs8_mask(5, tcr5);
tmr16_ctrl_mask_t tcnt_mask = {
    .ctrl_reg = &tcr5,
    .mask = 0
};

tmr16_ctrl_mask_t cap_pos_mask = {
    .ctrl_reg = &tcr5,
    .mask = (1 << ICES5),
};

void test_on(void) {
    pin_on(&b7);
}

void test_off(void) {
    pin_off(&b7);
}

ISR(TIMER4_COMPA_vect) {
    tmr16_event_call(&cha4);
    tmr16_int_disable(&cha4);
}

ISR(TIMER4_COMPB_vect) {
    tmr16_event_call(&chb4);
    tmr16_int_disable(&chb4);
}

ISR(TIMER4_COMPC_vect) {
    tmr16_event_call(&chc4);
    tmr16_int_disable(&chc4);
}

ISR(TIMER5_CAPT_vect) {
    tmr16_event_call(&cap5);
}

ISR(TIMER5_COMPA_vect) {
    tmr16_event_call(&cha5);
    tmr16_int_disable(&cha5);
}

ISR(TIMER5_COMPB_vect) {
    tmr16_event_call(&chb5);
    tmr16_int_disable(&chb5);
}

ISR(TIMER5_COMPC_vect) {
    tmr16_event_call(&chc5);
    tmr16_int_disable(&chc5);
}

ISR(TIMER5_OVF_vect) {
    tmr16_event_call(&ovf5);
}

uint16_t capture = 0;
uint8_t tooth_counter = 0;
bool tooth_counter_flag = 0;

void coil_on(void) {
    pin_on(&b5);
}

void coil_off(void) {
    pin_off(&b5);
}

typedef struct Coil_action{
    uint16_t new_angle;
    uint16_t old_angle;
    uint8_t tooth;
    timer_event action;
    struct Coil_action* next;
} coil_action_t;

coil_action_t coil14_on = {
    .action = coil_on,
    .tooth = 2
};

coil_action_t coil14_off = {
    .action = coil_off,
    .tooth = 20
};

coil_action_t* coil14_state;

void coil_action_handler(coil_action_t* coil) {
    if(coil->tooth == tooth_counter) {
        if(coil->action) coil->action();
        if(coil->new_angle != coil->old_angle) {
            
        }
        coil->old_angle = coil->new_angle;
        coil = coil->next;
    }
}

void main_handler() {
    coil_action_handler(coil14_state);
    tooth_counter++;
}

void capture_handler(void) {
    tmr16_counter_set(&tcnt_mask);
    tmr16_counter_set(&tcnt4_mask);
    capture = tmr16_read_cr(&cap5);
    tmr16_write_cr(&chc5, ((capture * 2)+(capture / 2))); //mark
    tmr16_int_enable(&chc5);
    if (tooth_counter_flag == true) main_handler();
    if (tooth_counter == 58) { //last tooth
        tmr16_write_cr(&cha5, capture); //59 tooth
        tmr16_int_enable(&cha5);
        tmr16_write_cr(&chb5, capture * 2); //60 tooth
        tmr16_int_enable(&chb5);
    }
    test_off();
}

void tooth_59_handler(void) {
    main_handler();
}

void tooth_60_handler(void) {
    main_handler();
}

void mark_handler(void) {
    tooth_counter = 0;
    tooth_counter_flag = true;
    test_on();
}

void stop_handler(void) {
    tooth_counter = 0;
    tooth_counter_flag = false;
}

int main() {
    sei();
    coil14_state = &coil14_on;
    coil14_on.next = &coil14_off;
    coil14_off.next = &coil14_on;
    pin_out(&b5);
    pin_out(&b6);
    pin_out(&b7);
    pin_in_pu(&l1);
    tmr16_set_cs(&tcs5);
    tmr16_set_cs(&tcs4);
    tmr16_capture_setup(&cap_pos_mask);
    tmr16_event_set(&cap5, capture_handler); //constant event
    tmr16_event_set(&cha5, tooth_59_handler); //constant event
    tmr16_event_set(&chb5, tooth_60_handler); //constant event
    tmr16_event_set(&chc5, mark_handler); //constant event
    tmr16_event_set(&ovf5, stop_handler); //constant event
    tmr16_int_enable(&cap5); //constant enabled interrupt
    tmr16_int_enable(&ovf5); //constant enabled interrupt
    while (1) {
        if (emu_tooth <= 57) pin_on(&b6);
        _delay_us(100);
        pin_off(&b6);
        _delay_us(100);
        if (emu_tooth <= 58) emu_tooth++;
        else emu_tooth = 0;
    }
}

