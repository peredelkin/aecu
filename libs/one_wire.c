#include "one_wire.h"

void one_wire_init(one_wire_t* port) {
    pin_in_pu(&port->one_wire_in);
    pin_out(&port->one_wire_out);
    pin_on(&port->one_wire_out);
}

#include <avr/io.h>
#include "libs/usart.h"
#include <util/delay.h>
#include "libs/ports.h"
#include "libs/one_wire.h"

uart_t uart0 = UART_init(0);
uint8_t adc_done = 0;

ISR(USART0_UDRE_vect) {
    uart_udre_handler(&uart0);
}

ISR(USART0_RX_vect) {
    uart_rx_handler(&uart0);
}

#define RSTL 500    //от 480
#define RSTH 400    //от 340
#define PDH 32      //от 15 до 60

#define REC 1           //Recovery Time
#define SLOT 120-REC     //Time Slot
#define W0L 60-REC      //Write 0 Low time
#define W0H SLOT-W0L    //Write 0 High time
#define W1L 10-REC       //Write 1 Low time
#define W1H SLOT-W1L    //Write 1 High time
#define RL 2-REC        //Read Low time
#define RH_1 10
#define RH SLOT-RH_1-RL      //Read High time

uint8_t one_wire_wait(one_wire_t* port) {
    uint16_t i = 1024;
    while (!(pin_val(&port->one_wire_in))) {
        if (i == 0) return 0;
    }
    return 1;
}

void one_wire_strobe(one_wire_t* port, uint8_t strobe) {
    if (strobe) {
        _delay_us(1);
        pin_on(&port->one_wire_out);
        _delay_us(1);
    } else {
        _delay_us(1);
        pin_off(&port->one_wire_out);
        _delay_us(1);
    }
}

void one_wire_write_bit(one_wire_t* port, uint8_t bit) {
    one_wire_strobe(port, 0);
    if (bit) _delay_us(7);
    else _delay_us(60);
    one_wire_strobe(port, 1);
    if (bit) _delay_us(93);
    else _delay_us(40);
}

uint8_t one_wire_read_bit(one_wire_t* port) {
    uint8_t bit = 0;
    one_wire_strobe(port, 0);
    _delay_us(2);
    one_wire_strobe(port, 1);
    _delay_us(2);
    bit = pin_val(&port->one_wire_in);
    _delay_us(44);
    return bit;
}

uint8_t one_wire_reset(one_wire_t* port) {
    uint16_t rsth = RSTH;
    one_wire_strobe(port, 0);
    _delay_us(RSTL);
    one_wire_strobe(port, 1);
    _delay_us(PDH);
    while (pin_val(&port->one_wire_in)) {
        if (rsth == 0) return 0;
        else rsth--;
    }
    return 1;
}

uint8_t one_wire_byte_tx(one_wire_t* port, uint8_t byte) {
    if (one_wire_wait(port)) {
        char i = 0;
        for (; i < 8; i++) {
            one_wire_write_bit(port, byte & 0x01);
            byte >>= 1;
        }
        return 1;
    } else return 0;
}

uint8_t one_wire_byte_rx(one_wire_t* port) {
    uint8_t byte = 0, i = 7;
    for (;;) {
        if (one_wire_read_bit(port)) {
            byte |= 0x80;
        }
        if (i == 0) break;
        i--;
        byte >>= 1;
    }
    return byte;
}

void one_wire_rx(one_wire_t* port, char* byte, char size) {
    uint8_t i = 0;
    for (; i < size; i++) {
        byte[i] = one_wire_byte_rx(port);
    }
}

uint8_t one_wire_read_two_bit(one_wire_t* port) {
    uint8_t bit = 0;
    bit = (one_wire_read_bit(port) << 1) | one_wire_read_bit(port);
    return bit;
}

one_wire_t ds18b20 = {
    make_pin(A, 0),
    make_pin(A, 1)
};

int main() {
    DDRC = 0xFF;
    one_wire_init(&ds18b20);
    uart_init(&uart0);
    uart_baud_rate(&uart0, UART_UBRR_VALUE);
    uart_character_size(&uart0, 8);
    uart_parity_mode(&uart0, UPM_DIS);
    uart_stop_bit(&uart0, 1);
    sei();
    uint8_t data;
    uint8_t data_full[8];
    uint8_t i = 0, n = 0;
    uint8_t step = 0;
    uint8_t mask = 0;
    _delay_ms(100);
    /*    while (1) {
            if (one_wire_reset(&ds18b20)) if (one_wire_byte_tx(&ds18b20, 0x33)) {
                    one_wire_rx(&ds18b20, data, 8);
                    uart_tx(&uart0, data, 8);
                }
            _delay_ms(100);
        }
     */
    while (step < 255) {
        data_full[0] = 0;
        data_full[1] = 0;
        data_full[2] = 0;
        data_full[3] = 0;
        data_full[4] = 0;
        data_full[5] = 0;
        data_full[6] = 0;
        data_full[7] = 0;
        mask = step;
        if (one_wire_reset(&ds18b20)) if (one_wire_byte_tx(&ds18b20, 0xF0)) {
                i = 0;
                n = 0;
                while (n < 8) {
                    data = one_wire_read_two_bit(&ds18b20);
                    if (data == 1) one_wire_write_bit(&ds18b20, 0);
                    if (data == 2) {
                        one_wire_write_bit(&ds18b20, 1);
                        data_full[n] |= (1 << i);
                    }
                    if (data == 0) {
                        if (mask & 0x01) {
                            one_wire_write_bit(&ds18b20, 1);
                            data_full[n] |= (1 << i);
                        } else one_wire_write_bit(&ds18b20, 0);
                        mask>>=1;
                    }
                    if (i == 7) {
                        i = 0;
                        n++;
                    } else i++;
                }
            }
        _delay_ms(1);
        uart_tx(&uart0, data_full, 8);
        step++;
    }
    while (1);
}
