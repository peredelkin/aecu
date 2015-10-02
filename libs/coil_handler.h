/* 
 * File:   coil_handler.h
 * Author: root
 *
 * Created on 2 Октябрь 2015 г., 19:39
 */

#ifndef COIL_HANDLER_H
#define	COIL_HANDLER_H

#include <stdint.h>
#include "timers.h"

typedef struct Coil_ch_act {
    struct Coil_ch_act* next;
    tmr16_int_ctrl_t ch;
} coil_ch_act_t;

typedef struct Coil_act {
    uint16_t angle_buffer; //new angle
    uint16_t angle; //old angle
    timer_event action;
    uint16_t tooth_angle;
    uint16_t action_angle;
    struct Coil_act* next;
    struct Coil_act* prev;
} coil_act_t;

#define make_coil_ch(N_CH,TIR,NEXT) { \
    .ch.cr = &TIMER_OCR(N_CH), \
    .ch.event = NULL, \
    .ch.flag = 0, \
    .ch.mask = (1 << TIMER_OCIE(N_CH)), \
    .ch.ti_register = &TIR, \
    .next = NEXT \
}

#define make_coil_act(ACTION,ANGLE) { \
    .action = ACTION, \
    .action_angle = ANGLE%6, \
    .angle = ANGLE, \
    .angle_buffer = ANGLE, \
    .next = NULL, \
    .prev = NULL, \
    .tooth_angle = ANGLE - ANGLE%6 \
}

#endif	/* COIL_HANDLER_H */

