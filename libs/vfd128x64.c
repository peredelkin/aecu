#include "vfd128x64.h"

void disp_stop(vfd_t* vfd) {
    pin_on(&vfd->cd);
    vfd->done = 1;
}

void disp_init(spi_t* spi_master, vfd_t* vfd) {
    pin_out(&vfd->cd);
    disp_stop(vfd);
    //spi_set_callback(spi_master, disp_callback);
    spi_init(spi_master, vfd->spi_clock_rate);
}

void disp_write_cmd(spi_t* spi_master, vfd_t* vfd, uint8_t cmd) {
    vfd->done = 0;
    spi_master_tx_byte(spi_master, cmd);
    while (!vfd->done);
}

void disp_write_data_byte(spi_t* spi_master, vfd_t* vfd, uint8_t data) {
    vfd->done = 0;
    pin_off(&vfd->cd);
    spi_master_tx_byte(spi_master, data);
    while (!vfd->done);
}

/*
void disp_write_buffer(spi_t* spi_master, vfd_t* vfd, graphics_t* graphics) {
    vfd->done = 0;
    pin_off(&vfd->cd);
    spi_master_tx(spi_master, graphics->buffer, WIDTH * HEIGHT * GRAY);
    while (!vfd->done);
}
*/