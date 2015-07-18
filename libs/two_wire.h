/* 
 * File:   two_wire.h
 * Author: ruslan
 *
 * Created on 18 октября 2014 г., 14:56
 */

#ifndef TWO_WIRE_H
#define	TWO_WIRE_H

#include <util/twi.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define TWI_SCL_VALUE 400000                                        //400kHz default
#define TWPS_VALUE 0 //Значение битов TWPS1 и TWPS0 от 0 до 3
#define PRESCAL_VAL 1 //Значени прескаллера (см. таблицу ниже)
#define TWBR_VALUE ((F_CPU/TWI_SCL_VALUE)-16)/(2*PRESCAL_VAL) //Значение TWBR

/*  TWI Bit Rate Prescaler
 * ==================================
 * TWPS1 | TWPS0 | Prescaler value  |
 * =================================
 *  0    |  0    |      1           |
 * =================================
 *  0    |  1    |      4           |
 * =================================
 *  1    |  0    |      16          |
 * =================================
 *  1    |  1    |      64          |
 * =================================
 */

typedef void (*twi_callback_t)(void);

typedef struct {
uint8_t twi_io; //Примем или передача.Передача,когда 0.
uint8_t twi_sla_addr; //Адрес устройства на шине.
uint8_t twi_sla_mem_addr; //Адрес в памяти устройства. 
uint8_t* twi_sla_data; //Принимаемые или для передачи данные.
uint8_t twi_sla_byte_n; //Переменная счетчика принимаемых байт.
uint8_t twi_size; //Количество принимаемых байт. 1 = 1 байту.
volatile bool twi_busy_flag; //Флаг занятости шины.
twi_callback_t callback; //
} twi_t;

extern void twi_init(twi_t *two_wire);
extern void twi_handler(twi_t *two_wire);
extern void twi_tx(twi_t *two_wire,uint8_t addr,uint8_t mem_addr,uint8_t *data,uint8_t size);
extern void twi_rx(twi_t *two_wire,uint8_t addr,uint8_t mem_addr,uint8_t *data,uint8_t size);
extern void twi_wait(twi_t* two_wire);
extern void twi_set_callback(twi_t* two_wire, twi_callback_t callback);

#endif	/* TWO_WIRE_H */

/* 
Пример для DS1307
#include <avr/io.h>
#include <util/delay.h>
#include "libs/two_wire.h"

twi_t i2c;

ISR(TWI_vect) {
    twi_handler(&i2c);
}


int main() {
    twi_init(&i2c);
    sei();
    uint8_t data[3]={0,0,0};
    uint8_t data_1[3];
    while (1) {
        twi_rx(&i2c,0x68,0,data_1,3);
        twi_tx(&i2c,0x68,0,data,3);
    }
}
 */