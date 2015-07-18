/* 
 * File:   vfd128x64.h
 * Author: root
 *
 * Created on 22 февраля 2015 г., 13:05
 */

#ifndef VFD128X64_H
#define	VFD128X64_H

#include "spi.h"
#include <string.h>
#include <stdint.h>
#include "vfd_graphics.h"

typedef struct {
    pin_t cd;
    uint8_t spi_clock_rate;
    volatile uint8_t done;
} vfd_t;

#define CMD_1_2_DISP_OFF        0x00
#define CMD_1_DISP_ON           0x01
#define CMD_2_DISP_ON           0x02

#define CMD_RW_ADDR_INCR        0x04
#define CMD_RW_ADDR_HELD        0x05

#define CMD_CLEAR_SCREEN        0x06

#define CMD_CTRL_PWR            0x07

#define CMD_DISP_WRITE          0x08
#define CMD_DISP_READ           0x09

#define CMD_1_SET_LO_ADDR       0x0A
#define CMD_1_SET_UP_ADDR       0x0B
#define CMD_2_SET_LO_ADDR       0x0C
#define CMD_2_SET_UP_ADDR       0x0D

#define CMD_SET_LO_ADDR_RW      0x0E
#define CMD_SET_UP_ADDR_RW      0x0F

#define CMD_OR_DISP             0x10
#define CMD_EX_OR_DISP          0x11
#define CMD_AND_DISP            0x12

#define CMD_LUM_ADJ             0x13
#define CMD_DISP_MODE           0x14
#define CMD_INT_MODE            0x15
#define CMD_CHAR_DISP           0x20
#define CMD_SET_CHAR_LOC_RAM    0x21
#define CMD_SET_CHAR_SIZE       0x22
#define CMD_SET_CHAR_BRIGHT     0x24

extern void disp_stop(vfd_t* vfd);
extern void disp_init(spi_t* spi_master, vfd_t* vfd);
extern void disp_write_cmd(spi_t* spi_master, vfd_t* vfd, uint8_t cmd);
extern void disp_write_data_byte(spi_t* spi_master, vfd_t* vfd, uint8_t data);
//extern void disp_write_buffer(spi_t* spi_master, vfd_t* vfd, graphics_t* graphics);

static void disp_write_buffer(spi_t* spi_master, vfd_t* vfd, graphics_t* graphics) {
    if(vfd->done) {
    disp_write_cmd(spi_master, vfd, CMD_DISP_WRITE);
    vfd->done = 0;
    pin_off(&vfd->cd);
    spi_master_tx(spi_master, graphics->buffer, WIDTH * HEIGHT * GRAY);
    //while(!vfd->done);
    }
}

#endif	/* VFD128X64_H */

