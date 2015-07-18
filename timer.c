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

