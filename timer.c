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

pin_t b4 = make_pin(B, 4); //coil out
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
    tmr16_event_call(&cha4);
    tmr16_event_set(&cha4, NULL);
    tmr16_int_disable(&cha4);
}

ISR(TIMER4_COMPB_vect) {
    tmr16_event_call(&chb4);
    tmr16_event_set(&chb4, NULL);
    tmr16_int_disable(&chb4);
}

ISR(TIMER4_COMPC_vect) {
    tmr16_event_call(&chc4);
    tmr16_event_set(&chc4, NULL);
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
uint16_t angle_counter = 0;
bool tooth_counter_flag = 0;

static void coil14on(void) {
    pin_on(&b5);
}

static void coil14off(void) {
    pin_off(&b5);
}

static void coil23on(void) {
    pin_on(&b4);
}

static void coil23off(void) {
    pin_off(&b4);
}

typedef struct Coil_action {
    uint16_t new_angle;
    uint16_t old_angle;
    uint16_t tooth_angle;
    uint16_t action_angle;
    timer_event action;
    struct Coil_action* next;
} coil_action_t;

#define make_coil(ANGLE,ACTION) { \
    .action = ACTION, \
    .action_angle = ANGLE%6, \
    .tooth_angle = ANGLE - ANGLE%6, \
    .new_angle = ANGLE, \
    .old_angle = ANGLE \
}

coil_action_t coil14_on = make_coil(0, coil14on);
coil_action_t coil14_off = make_coil(114, coil14off);
coil_action_t coil23_on = make_coil(180, coil23on);
coil_action_t coil23_off = make_coil(294, coil23off);

coil_action_t* coil_state;

static void coil_action_handler(coil_action_t** coil) {
    if ((*coil)->tooth_angle == angle_counter) {
        if ((*coil)->action_angle == 0) (*coil)->action();
        else {
            tmr16_write_cr(&cha4, (capture * ((*coil)->action_angle)) / 6);
            tmr16_event_set(&cha4, (*coil)->action);
            tmr16_int_enable(&cha4);
        }
        if ((*coil)->old_angle != (*coil)->new_angle) {
            (*coil)->action_angle = (*coil)->new_angle % 6;
            (*coil)->tooth_angle = (*coil)->new_angle - (*coil)->action_angle;
        }
        (*coil)->old_angle = (*coil)->new_angle;
        *coil = (*coil)->next;
    }
}

static void main_handler() {
    coil_action_handler(&coil_state);
    angle_counter += 6;
}

static void capture_handler(void) {
    tmr16_counter_set(&tcnt5_mask);
    tmr16_counter_set(&tcnt4_mask);
    capture = tmr16_read_cr(&cap5);
    tmr16_write_cr(&chc5, ((capture * 2)+(capture / 2))); //mark
    tmr16_int_enable(&chc5);
    if (tooth_counter_flag == true) {
        main_handler();
        if (angle_counter == 348) { //last tooth 58*6
            tmr16_write_cr(&cha5, capture); //59 tooth
            tmr16_int_enable(&cha5);
            tmr16_write_cr(&chb5, capture * 2); //60 tooth
            tmr16_int_enable(&chb5);
        }
    }
    test_off();
}

static void tooth_59_handler(void) {
    tmr16_counter_set(&tcnt4_mask);
    main_handler();
}

static void tooth_60_handler(void) {
    tmr16_counter_set(&tcnt5_mask);
    tmr16_counter_set(&tcnt4_mask);
    main_handler();
    angle_counter = 0;
    test_on();
}

static void mark_handler(void) {
    angle_counter = 0;
    tooth_counter_flag = true; //start
    test_on();
}

static void stop_handler(void) {
    angle_counter = 0;
    tooth_counter_flag = false; //stop
    test_on();
}

int16_t lerp(int16_t a, int16_t b, uint16_t t) {
    return a + ((b - a) * t) / 256;
}

int16_t bilerp(int16_t a1, int16_t b1, int16_t a2, int16_t b2, uint16_t t1, uint16_t t2) {
    int16_t a = lerp(a1, b1, t1);
    int16_t b = lerp(a2, b2, t1);
    return lerp(a, b, t2);
}

int16_t bilerp_map(int16_t *map, uint16_t x_val, uint16_t y_val) {
    uint16_t x = x_val / 256;
    uint16_t y = y_val / 256;
    uint16_t tx = x_val % 256;
    uint16_t ty = y_val % 256;
    return bilerp(
            map[2*y+x],
            map[2*y+(x+1)],
            map[2*(y+1)+x],
            map[2*(y+1)+(x+1)],
            tx, ty
            );
}

int16_t lerp_map[4] = {
    5,10,0,5
};

static void calc_ignition_angle(void) {
    int16_t calc_angle = bilerp_map(lerp_map, 128,1);
    coil14_on.new_angle = (360 - calc_angle) % 360; //!!!
    coil14_off.new_angle = (474 - calc_angle) % 360; //!!!
    coil23_on.new_angle = (coil14_on.new_angle + 180) % 360; //!!!
    coil23_off.new_angle = (coil14_off.new_angle + 180) % 360; //!!!
}

int main() {
    sei();
    pin_out(&b4);
    pin_out(&b5);
    pin_out(&b6);
    pin_out(&b7);
    coil_state = &coil14_on;
    coil14_on.next = &coil14_off;
    coil14_off.next = &coil23_on;
    coil23_on.next = &coil23_off;
    coil23_off.next = &coil14_on;
    pin_in_pu(&l1);
    tmr16_set_cs(&tcs5);
    tmr16_set_cs(&tcs4);
    tmr16_capture_setup(&cap5_pos_mask);
    tmr16_event_set(&cap5, capture_handler); //constant event
    tmr16_event_set(&cha5, tooth_59_handler); //constant event
    tmr16_event_set(&chb5, tooth_60_handler); //constant event
    tmr16_event_set(&chc5, mark_handler); //constant event
    tmr16_event_set(&ovf5, stop_handler); //constant event
    tmr16_int_enable(&cap5); //constant enabled interrupt
    tmr16_int_enable(&ovf5); //constant enabled interrupt
    while (1) {
        if (emu_tooth <= 57) pin_on(&b6);
        _delay_us(500);
        pin_off(&b6);
        _delay_us(500);
        if (emu_tooth <= 58) emu_tooth++;
        else {
            calc_ignition_angle();
            emu_tooth = 0;
        }
    }
}
