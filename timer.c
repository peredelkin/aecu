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

pin_t b6 = make_pin(B, 6); //emu out
pin_t b7 = make_pin(B, 7); //test out
pin_t l1 = make_pin(L, 1); //capture in
uint8_t emu_tooth = 0;

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

void test_on(void) {
    pin_on(&b7);
}

void test_off(void) {
    pin_off(&b7);
}

ISR(TIMER5_CAPT_vect) {
    tmr16_event_call(&cap5);
}

ISR(TIMER5_COMPA_vect) {
    tmr16_event_call(&cha5);
}

ISR(TIMER5_COMPB_vect) {
    tmr16_event_call(&chb5);
}

ISR(TIMER5_COMPC_vect) {
    tmr16_event_call(&chc5);
}

ISR(TIMER5_OVF_vect) {
    tmr16_event_call(&ovf5);
}

uint8_t tooth_counter = 0;
bool tooth_counter_flag = 0;

void capture_handler(void) {
    if (tooth_counter_flag == true) tooth_counter++;
    tmr16_counter_set(&tcnt_mask);
    uint16_t capture = tmr16_read_cr(&cap5);
    tmr16_write_cr(&chc5, ((capture * 2)+(capture / 2)));
    tmr16_int_enable(&chc5);
    if(tooth_counter == 58) {
        tmr16_write_cr(&cha5,capture);
        tmr16_int_enable(&cha5);
        tmr16_write_cr(&chb5,capture*2);
        tmr16_int_enable(&chb5);
    }
    test_off();
}

void tooth_59_handler(void) {
    tmr16_int_disable(&cha5);
}

void tooth_60_handler(void) {
    tmr16_int_disable(&chb5);
}

void mark_handler(void) {
    tmr16_int_disable(&chc5);
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
    pin_out(&b6);
    pin_out(&b7);
    pin_in_pu(&l1);
    tmr16_set_cs(&tcs5);
    tmr16_event_set(&cap5, capture_handler);
    tmr16_int_enable(&cap5);
    tmr16_event_set(&cha5, tooth_59_handler);
    tmr16_event_set(&chb5, tooth_60_handler);
    tmr16_event_set(&chc5, mark_handler);
    tmr16_event_set(&ovf5, stop_handler);
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

