#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "libs/coil_handler.h"
#include "libs/ports.h"
#include "libs/usart.h"
#include <string.h>

#define UART_BAUD 57600
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
tmr16_ctrl_mask_t tcnt4_mask = {
    .ctrl_reg = &tcr4,
    .mask = 0
};

ISR(TIMER4_COMPA_vect) {
    coil_call_event_once(&t4ch1);
}

ISR(TIMER4_COMPB_vect) {
    coil_call_event_once(&t4ch2);
}

ISR(TIMER4_COMPC_vect) {
    coil_call_event_once(&t4ch3);
}

pin_t b7 = make_pin(B, 7); //test out

void test_on(void) {
    pin_on(&b7);
}

void test_off(void) {
    pin_off(&b7);
}

coil_act_t root = make_coil_act(NULL, 0);
coil_act_t teston = make_coil_act(test_on, 180);
coil_act_t testoff = make_coil_act(test_off, 182);
coil_act_t* coil_buffer;

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
    tmr16_set_cs(&tcs4);
    while (!(uart_tx_done(&uart0)));
    sprintf(data, "Timer inited\n");
    uart_tx(&uart0, data, strlen(data));
}

void port_main_init(void) {
    pin_out(&b7);
    while (!(uart_tx_done(&uart0)));
    sprintf(data, "Port inited\n");
    uart_tx(&uart0, data, strlen(data));
}

void action_main_init(void) {
    coil_act_sorting_insert(&root, &teston);
    coil_act_sorting_insert(&root, &testoff);
    while (!(uart_tx_done(&uart0)));
    sprintf(data, "Action inited\n");
    uart_tx(&uart0, data, strlen(data));
}

uint16_t counter = 0;

int main() {
    sei();
    uart_main_init(&uart0);
    timer_main_init();
    port_main_init();
    action_main_init();
    while (1) {
        _delay_ms(5);
        if (counter >= 359) {
            counter = 0;
            if(testoff.angle_buffer >= 360) testoff.angle_buffer = 0;
            else testoff.angle_buffer++;
        } else counter += 6;
        coil_act_handler(&coil_buffer,&root,&t4ch1,&tcnt4_mask,20000,counter);
    }
}
