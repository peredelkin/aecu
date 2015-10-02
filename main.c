#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "libs/coil_handler.h"
#include "libs/timers.h"
#include "libs/ports.h"
#include "libs/usart.h"

#define UART_BAUD 76800
#define UART_UBRR_VALUE (F_CPU / 16 / UART_BAUD - 1)

uart_t uart0 = UART_init(0);
uint8_t done=0;

ISR(USART0_UDRE_vect)
{
    uart_udre_handler(&uart0);
}

ISR(USART0_RX_vect)
{
    uart_rx_handler(&uart0);
}

//===================================== usart

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

coil_act_t coil14_on = make_coil_act(coil14on,0);
coil_act_t coil14_off = make_coil_act(coil14off,114);
coil_act_t coil23_on = make_coil_act(coil23on,180);
coil_act_t coil23_off = make_coil_act(coil23off,294);
coil_act_t* coil_state;

uint16_t capture = 0;
uint16_t angle_counter = 0;
bool tooth_counter_flag = 0;

static void main_handler() {
    coil_act_main_handler(&coil_state,&coil_ch1,angle_counter,capture);
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
    return a+(int16_t)((((int32_t)b - (int32_t)a) * (int32_t)t)/256);
}

int16_t bilerp(int16_t a1, int16_t b1, int16_t a2, int16_t b2, uint16_t t1, uint16_t t2) {
    int16_t a = lerp(a1, b1, t1);
    int16_t b = lerp(a2, b2, t1);
    return lerp(a, b, t2);
}

int16_t bilerp_map(int8_t (*map)[40], uint16_t x_val, uint16_t y_val) {
    uint16_t x = x_val / 256;
    uint16_t y = y_val / 256;
    uint16_t tx = x_val % 256;
    uint16_t ty = y_val % 256;
    return bilerp(
            (int16_t)map[y][x],
            (int16_t)map[y][x+1],
            (int16_t)map[y+1][x],
            (int16_t)map[y+1][x+1],
            tx, ty
            );
}

int8_t lerp_map[40][40] = {
    {5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100},
    {-5,0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95}
};
char data[30];

int16_t off_angle;
int16_t on_angle;
uint32_t time;
uint32_t rpm;

#define ms 25 // 2.5ms = 25

static void calc_ignition_angle(void) {
    time = (uint32_t)ms * 1200;
    on_angle = time / capture;
    rpm = 2000000/capture;
    off_angle = bilerp_map(lerp_map, (uint16_t)rpm,0);
    if(uart_tx_done(&uart0)) sprintf(data,"RPM %d \n",(uint16_t)rpm);
    coil14_on.angle_buffer = (473 - off_angle - on_angle) % 360; //!!!
    coil14_off.angle_buffer = (474 - off_angle) % 360; //!!!
    coil23_on.angle_buffer = (coil14_on.angle_buffer + 180) % 360; //!!!
    coil23_off.angle_buffer = (coil14_off.angle_buffer + 180) % 360; //!!!
}

int main() {
    //uart
    uart_init(&uart0);
    uart_baud_rate(&uart0,UART_UBRR_VALUE);
    uart_character_size(&uart0,8);
    uart_parity_mode(&uart0,UPM_DIS);
    uart_stop_bit(&uart0,1);
    //uart
    sei();
    pin_out(&b4);
    pin_out(&b5);
    pin_out(&b6);
    pin_out(&b7);
    coil_state = &coil14_on;
    coil_act_sorting_insert(&coil14_on,&coil14_off);
    coil_act_sorting_insert(&coil14_on,&coil23_on);
    coil_act_sorting_insert(&coil14_on,&coil23_off);
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
        _delay_us(600);
        pin_off(&b6);
        _delay_us(600);
        if (emu_tooth <= 58) emu_tooth++;
        else {
            calc_ignition_angle();
            if(uart_tx_done(&uart0)) uart_tx(&uart0, data, 30);
            emu_tooth = 0;
        }
    }
}
