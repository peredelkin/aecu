#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "libs/coil_handler.h"
#include "libs/ports.h"
#include "libs/usart.h"
#include <string.h>

#define UART_BAUD 460800
#define UART_UBRR_VALUE (F_CPU / 16 / UART_BAUD - 1)

uart_t uart0 = UART_init(0);
uint8_t done = 0;

ISR(USART0_UDRE_vect) {
    uart_udre_handler(&uart0);
}

ISR(USART0_RX_vect) {
    uart_rx_handler(&uart0);
}

char data[30];

//=========================================

tmr16_int_reg_t tir4 = make_tir(4);
coil_ch_act_t t4ch3 = make_coil_ch(4C, tir4, NULL);
coil_ch_act_t t4ch2 = make_coil_ch(4B, tir4, &t4ch3);
coil_ch_act_t t4ch1 = make_coil_ch(4A, tir4, &t4ch2);
tmr16_ctrl_reg_t tcr4 = make_tcr(4);
tmr16_ctrl_mask_t tcs4 = make_cs8_mask(4, tcr4);
tmr16_ctrl_mask_t tcnt4_mask = make_tcnt_mask(0, tcr4);

tmr16_int_reg_t tir5 = make_tir(5);
tmr16_int_ctrl_t cap5 = make_cap(5, tir5);
tmr16_int_ctrl_t cha5 = make_ch(5A, tir5);
tmr16_int_ctrl_t chb5 = make_ch(5B, tir5);
tmr16_int_ctrl_t chc5 = make_ch(5C, tir5);
tmr16_int_ctrl_t ovf5 = make_ovf(5, tir5);
tmr16_ctrl_reg_t tcr5 = make_tcr(5);
tmr16_ctrl_mask_t tcs5 = make_cs8_mask(5, tcr5);
tmr16_ctrl_mask_t tcnt5_mask = make_tcnt_mask(0, tcr5);
tmr16_ctrl_mask_t cap5_pos_mask = {
    .ctrl_reg = &tcr5,
    .mask = (1 << ICES5),
};

ISR(TIMER4_COMPA_vect) { //coil interrupt
    coil_call_event_once(&t4ch1);
    if (uart_tx_done(&uart0)) {
        sprintf(data, "TC4A\n");
        uart_tx(&uart0, data, strlen(data));
    }
}

ISR(TIMER4_COMPB_vect) { //coil interrupt
    coil_call_event_once(&t4ch2);
    if (uart_tx_done(&uart0)) {
        sprintf(data, "TC4B\n");
        uart_tx(&uart0, data, strlen(data));
    }
}

ISR(TIMER4_COMPC_vect) { //coil interrupt
    coil_call_event_once(&t4ch3);
    if (uart_tx_done(&uart0)) {
        sprintf(data, "TC4C\n");
        uart_tx(&uart0, data, strlen(data));
    }
}

ISR(TIMER5_CAPT_vect) { //capture main handler interrupt
    tmr16_event_call(&cap5);
}

ISR(TIMER5_COMPA_vect) { //tooth 58 interrupt
    tmr16_event_call(&cha5);
    tmr16_int_disable(&cha5);
}

ISR(TIMER5_COMPB_vect) { //tooth 59 interrupt
    tmr16_event_call(&chb5);
    tmr16_int_disable(&chb5);
}

ISR(TIMER5_COMPC_vect) { //mark interrupt
    tmr16_event_call(&chc5);
    tmr16_int_disable(&chc5);
}

ISR(TIMER5_OVF_vect) { //stop interrupt
    tmr16_event_call(&ovf5);
    tmr16_int_disable(&ovf5);
}

pin_t b4 = make_pin(B, 4); //coil14 out
pin_t b5 = make_pin(B, 5); //coil23 out
pin_t b6 = make_pin(B, 6); //emu out
pin_t b7 = make_pin(B, 7); //test out
pin_t l1 = make_pin(L, 1); //capture in

void test_on(void) { // test/mark
    pin_on(&b7);
}

void test_off(void) { // test/mark
    pin_off(&b7);
}

void coil14_on(void) {
    pin_on(&b4);
}

void coil14_off(void) {
    pin_off(&b4);
}

void coil23_on(void) {
    pin_on(&b5);
}

void coil23_off(void) {
    pin_off(&b5);
}

coil_act_t root = make_coil_act(NULL, 0);
coil_act_t coil14on = make_coil_act(coil14_on, 1);
coil_act_t coil14off = make_coil_act(coil14_off, 5);
coil_act_t coil23on = make_coil_act(coil23_on, 181);
coil_act_t coil23off = make_coil_act(coil23_off, 185);
coil_act_t* coil_buffer = &root;

volatile uint16_t timer_capture = 0;
volatile uint16_t tooth_counter = 0;
volatile bool permit_tooth_counter = 0;

void main_handler(void) {
    coil_act_handler(&coil_buffer, &root, &t4ch1, &tcnt4_mask, timer_capture, tooth_counter);
    tooth_counter += 6;
}

uint16_t current_capture(uint16_t penult_capture, uint16_t last_capture) {
    int32_t penult = (int32_t) penult_capture,
            last = (int32_t) last_capture,
            current;
    current = last * 2 - penult;
    if (current <= 0) {
        return 0;
    } else {
        if (current >= 0xffff) {
            return 0xffff;
        } else {
            return (uint16_t) current;
        }
    }
}

void capture_handler(void) {
    tmr16_counter_set(&tcnt5_mask);
    timer_capture = tmr16_read_cr(&cap5);
    tmr16_write_cr(&chc5, ((timer_capture * 2)+(timer_capture / 2))); //mark
    tmr16_int_enable(&chc5);
    tmr16_int_enable(&ovf5);
    if (permit_tooth_counter == true) {
        main_handler();
        if (tooth_counter == 348) { //last tooth 58*6
            tmr16_write_cr(&cha5, timer_capture); //59 tooth
            tmr16_int_enable(&cha5);
            tmr16_write_cr(&chb5, timer_capture * 2); //60 tooth
            tmr16_int_enable(&chb5);
        }
    }
}

void tooth_59_handler(void) {
    main_handler();
    test_on();
}

void tooth_60_handler(void) {
    tmr16_counter_set(&tcnt5_mask);
    main_handler();
    tooth_counter = 0;
    test_off();
}

void mark_handler(void) {
    tooth_counter = 0;
    permit_tooth_counter = true;
    if (uart_tx_done(&uart0)) {
        sprintf(data, "Mark\n");
        uart_tx(&uart0, data, strlen(data));
    }
}

void stop_handler(void) {
    tooth_counter = 0;
    permit_tooth_counter = false;
    test_off();
    coil14_off();
    coil23_off();
    if (uart_tx_done(&uart0)) {
        sprintf(data, "Stop\n");
        uart_tx(&uart0, data, strlen(data));
    }
}

void uart_main_init(uart_t* uart) {
    uart_init(uart);
    uart_baud_rate(uart, UART_UBRR_VALUE);
    uart_character_size(uart, 8);
    uart_parity_mode(uart, UPM_DIS);
    uart_stop_bit(uart, 1);
    while (!(uart_tx_done(uart)));
    sprintf(data, "Uart inited\n");
    uart_tx(uart, data, strlen(data));
}

void timer_main_init(void) {
    tmr16_set_cs(&tcs4); //action timer
    tmr16_set_cs(&tcs5); //capture timer
    tmr16_capture_setup(&cap5_pos_mask);
    tmr16_event_set(&cap5, capture_handler);
    tmr16_event_set(&cha5, tooth_59_handler);
    tmr16_event_set(&chb5, tooth_60_handler);
    tmr16_event_set(&chc5, mark_handler);
    tmr16_event_set(&ovf5, stop_handler);
    tmr16_int_enable(&cap5);
    tmr16_int_enable(&ovf5);
    while (!(uart_tx_done(&uart0)));
    sprintf(data, "Timer inited\n");
    uart_tx(&uart0, data, strlen(data));
}

void port_main_init(void) {
    pin_out(&b4);
    pin_out(&b5);
    pin_out(&b6);
    pin_out(&b7);
    pin_in_pu(&l1);
    while (!(uart_tx_done(&uart0)));
    sprintf(data, "Port inited\n");
    uart_tx(&uart0, data, strlen(data));
}

void action_main_init(void) {
    coil_act_sorting_insert(&root, &coil14on);
    coil_act_sorting_insert(&root, &coil14off);
    coil_act_sorting_insert(&root, &coil23on);
    coil_act_sorting_insert(&root, &coil23off);
    while (!(uart_tx_done(&uart0)));
    sprintf(data, "Action inited\n");
    uart_tx(&uart0, data, strlen(data));
}

uint8_t emu_tooth = 0;

void emu_sheave(void) {
    if (emu_tooth <= 57) pin_on(&b6);
    _delay_us(600);
    pin_off(&b6);
    _delay_us(600);
    if (emu_tooth <= 58) emu_tooth++;
    else {
        emu_tooth = 0;
    }
}

int main() {
    sei();
    uart_main_init(&uart0);
    timer_main_init();
    port_main_init();
    action_main_init();
    while (1) {
        emu_sheave();
        if(uart_rx_done(&uart0)) {
            
        }
    }
}