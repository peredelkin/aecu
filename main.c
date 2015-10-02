#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "libs/coil_handler.h"
#include "libs/timers.h"
#include "libs/ports.h"
#include "libs/usart.h"

pin_t b4 = make_pin(B, 4); //coil out
pin_t b5 = make_pin(B, 5); //coil out
pin_t b6 = make_pin(B, 6); //emu out
pin_t b7 = make_pin(B, 7); //test out
pin_t l1 = make_pin(L, 1); //capture in
uint8_t emu_tooth = 0;

tmr16_int_reg_t tir4 = make_tir(4);
tmr16_ctrl_reg_t tcr4 = make_tcr(4);
tmr16_ctrl_mask_t tcs4 = make_cs8_mask(4, tcr4);
tmr16_ctrl_mask_t tcnt4_mask = {
    .ctrl_reg = &tcr4,
    .mask = 0
};

coil_ch_act_t coil_ch3 = make_coil_ch(4C, tir4, NULL);
coil_ch_act_t coil_ch2 = make_coil_ch(4B, tir4, &coil_ch3);
coil_ch_act_t coil_ch1 = make_coil_ch(4A, tir4, &coil_ch2);

tmr16_int_reg_t tir5 = make_tir(5);
tmr16_int_ctrl_t cap5 = make_cap(5, tir5);
tmr16_int_ctrl_t cha5 = make_ch(5A, tir5);
tmr16_int_ctrl_t chb5 = make_ch(5B, tir5);
tmr16_int_ctrl_t chc5 = make_ch(5C, tir5);
tmr16_int_ctrl_t ovf5 = make_ovf(5, tir5);
tmr16_ctrl_reg_t tcr5 = make_tcr(5);
tmr16_ctrl_mask_t tcs5 = make_cs8_mask(5, tcr5);
tmr16_ctrl_mask_t tcnt5_mask = {
    .ctrl_reg = &tcr5,
    .mask = 0
};

tmr16_ctrl_mask_t cap5_pos_mask = {
    .ctrl_reg = &tcr5,
    .mask = (1 << ICES5),
};

static void test_on(void) {
    pin_on(&b7);
}

static void test_off(void) {
    pin_off(&b7);
}

ISR(TIMER4_COMPA_vect) {
    tmr16_event_call(&coil_ch1.ch);
    tmr16_event_set(&coil_ch1.ch, NULL);
    tmr16_int_disable(&coil_ch1.ch);
}

ISR(TIMER4_COMPB_vect) {
    tmr16_event_call(&coil_ch2.ch);
    tmr16_event_set(&coil_ch2.ch, NULL);
    tmr16_int_disable(&coil_ch2.ch);
}

ISR(TIMER4_COMPC_vect) {
    tmr16_event_call(&coil_ch3.ch);
    tmr16_event_set(&coil_ch3.ch, NULL);
    tmr16_int_disable(&coil_ch3.ch);
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

int main() {
    while (1) {
        
    }
}
