#include "spi.h"

void spi_init(spi_t* spi_master, uint8_t spi_clock_rate) {

    pin_in(&spi_master->spi_miso);
    pin_out(&spi_master->spi_mosi);
    pin_out(&spi_master->spi_sck);
    pin_out(&spi_master->spi_ss);

    SPCR |= (1 << SPIE); // SPI Interrupt Enable
    SPCR |= (1 << DORD); // Data Order (LSB first,if set)
    SPCR |= (1 << MSTR); //  Master/Slave Select (Master,if set)
    SPCR |= (1 << CPOL); // Rising/Falling by Leading Edge (Falling,if set)
    SPCR |= (1 << CPHA); // Sample/Setup by  Leading Edge (Setup,if set)

    if (spi_clock_rate) {
        if ((spi_clock_rate % 10) && ((spi_clock_rate % 10) < 4)) SPCR |= (spi_clock_rate % 10);
        if ((spi_clock_rate / 10) % 10) SPSR |= (1 << SPI2X);
    }

    SPCR |= (1 << SPE); // SPI Enable

    sei();

    pin_on(&spi_master->spi_ss);
}

void spi_set_callback(spi_t* spi_master, spi_callback_t callback) {
    spi_master->callback = callback;
}

/*
void spi_master_handler(spi_t* spi_master) {
    if (spi_master->spi_io) {
        if (spi_master->spi_byte_n < spi_master->spi_size) {
            SPDR = spi_master->spi_data[spi_master->spi_byte_n];
            spi_master->spi_byte_n++;
        } else {
            pin_on(&spi_master->spi_ss);
            if (spi_master->callback) spi_master->callback();
        }
    } else {
        spi_master->spi_data[spi_master->spi_byte_n] = SPDR;
        spi_master->spi_byte_n++;
        if (spi_master->spi_byte_n < spi_master->spi_size) {
            SPDR = 0xFF;
        } else {
            pin_on(&spi_master->spi_ss);
            if (spi_master->callback) spi_master->callback();
        }
    }
}
 */

void spi_master_rx(spi_t* spi_master, uint8_t* data, uint16_t size) {
    pin_off(&spi_master->spi_ss);
    spi_master->spi_byte_n = 0;
    spi_master->spi_io = 0;
    spi_master->spi_size = size;
    spi_master->spi_data = data;
    SPDR = 0xFF;
}

void spi_master_tx(spi_t* spi_master, uint8_t* data, uint16_t size) {
    pin_off(&spi_master->spi_ss);
    spi_master->spi_byte_n = 1;
    spi_master->spi_io = 1;
    spi_master->spi_size = size;
    spi_master->spi_data = data;
    SPDR = data[0];
}

void spi_master_tx_byte(spi_t* spi_master, uint8_t data) {
    pin_off(&spi_master->spi_ss);
    spi_master->spi_byte_n = 1;
    spi_master->spi_io = 1;
    spi_master->spi_size = 1;
    SPDR = data;
}
