/* 
 * File:   spi.h
 * Author: root
 *
 * Created on 19 февраля 2015 г., 22:40
 */

#ifndef SPI_H
#define	SPI_H

#include <avr/interrupt.h>
#include "ports.h"

#define SPI_CLOCK_RATE_4    0
#define SPI_CLOCK_RATE_16   1
#define SPI_CLOCK_RATE_64   2
#define SPI_CLOCK_RATE_128  3

#define SPI_CLOCK_RATE_4X2    10
#define SPI_CLOCK_RATE_16X2   11
#define SPI_CLOCK_RATE_64X2   12
#define SPI_CLOCK_RATE_128X2  13

typedef void (*spi_callback_t)(void);

typedef struct {
    pin_t spi_ss;
    pin_t spi_sck;
    pin_t spi_mosi;
    pin_t spi_miso;
    uint8_t spi_io; // R/W
    uint16_t spi_byte_n; //Счетчик байт
    uint16_t spi_size; //Количество байт
    uint8_t* spi_data; //Данные
    spi_callback_t callback;
} spi_t;

extern void spi_init(spi_t* spi_master, uint8_t spi_clock_rate);
extern void spi_set_callback(spi_t* spi_master, spi_callback_t callback);
//extern void spi_master_handler(spi_t* spi_master);

static void spi_master_handler(spi_t* spi_master) {
    if (spi_master->spi_io) {
        if (spi_master->spi_byte_n < spi_master->spi_size) {
            SPDR = spi_master->spi_data[spi_master->spi_byte_n];
            spi_master->spi_byte_n++;
        } else {
            pin_on(&spi_master->spi_ss);
            if (spi_master->callback) spi_master->callback();
        }
    } else {
        if (spi_master->spi_byte_n < spi_master->spi_size) {
            spi_master->spi_data[spi_master->spi_byte_n] = SPDR;
            spi_master->spi_byte_n++;
            SPDR = 0xFF;
        } else {
            pin_on(&spi_master->spi_ss);
            if (spi_master->callback) spi_master->callback();
        }
    }
}

extern void spi_master_rx(spi_t* spi_master, uint8_t* data, uint16_t size);
extern void spi_master_tx(spi_t* spi_master, uint8_t* data, uint16_t size);
extern void spi_master_tx_byte(spi_t* spi_master, uint8_t data);

#endif	/* SPI_H */



/*
#include <avr/io.h>
#include <avr/interrupt.h>
#include "libs/ports.h"
#include "libs/spi.h"

spi_t spi={make_pin(B,0),make_pin(B,1),make_pin(B,2),make_pin(B,3)};

ISR(SPI_STC_vect) {
    spi_master_handler(&spi);
}

int main() {
    spi_init(&spi,SPI_CLOCK_RATE_128);
    spi_master_tx_byte(&spi,0x8);
    while(1);
}
 */