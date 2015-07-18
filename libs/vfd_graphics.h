/* 
 * File:   vfd_graphics.h
 * Author: root
 *
 * Created on 23 февраля 2015 г., 3:47
 */

#ifndef VFD_GRAPHICS_H
#define	VFD_GRAPHICS_H

#include <stdint.h>
#include <string.h>

#define HEIGHT 64/8
#define WIDTH 128
#define SET_GRAY

#ifdef SET_GRAY
#define GRAY 2
#else
#define GRAY 1
#endif

typedef struct {
    uint8_t buffer[WIDTH*HEIGHT*GRAY];
} graphics_t;

static void set_pixel(graphics_t* graphics, int16_t x, int16_t y, uint8_t val) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT * 8) return;
    graphics->buffer[(16 * x)+(y / 4)] |= (val << 6 - (y * 2) % 8);
}

static void set_byte(graphics_t* graphics, int16_t x, int16_t line, uint8_t val) {
    if (x < 0 || x >= WIDTH || line < 0 || line >= HEIGHT * 2) return;
    graphics->buffer[(16 * x)+(line)] |= 0x55 * val;
}

static void h_line(graphics_t* graphics, int16_t x_from, int16_t x_to, int16_t y, uint8_t val) {
    if (y < 0 || y >= WIDTH) return;

    if (x_from >= WIDTH) x_from = WIDTH;
    else if (x_from < 0) x_from = -1;

    if (x_to >= WIDTH) x_to = WIDTH;
    else if (x_to < 0) x_to = -1;

    while (x_from != x_to) {
        x_to--;
        set_pixel(graphics, x_to, y, val);
    }
}

static void draw_h_line(graphics_t* graphics, int16_t x_from, int16_t x_to, int16_t y, uint8_t val) {
    if (x_from <= x_to) h_line(graphics, x_from, x_to, y, val);
    else h_line(graphics, x_to, x_from, y, val);
}

static void v_line(graphics_t* graphics, int16_t x, int16_t y_from, int16_t y_to, uint8_t val) {
    if (x < 0 || x >= WIDTH) return;

    if (y_from >= HEIGHT * 8) y_from = HEIGHT * 8;
    else if (y_from < 0) y_from = -1;

    if (y_to >= HEIGHT * 8) y_to = HEIGHT * 8;
    else if (y_to < 0) y_to = -1;

    while (y_from != y_to) {
        y_to--;
        set_pixel(graphics, x, y_to, val);
    }
}

static void draw_v_line(graphics_t* graphics, int16_t x, int16_t y_from, int16_t y_to, uint8_t val) {
    if (y_from <= y_to) v_line(graphics, x, y_from, y_to, val);
    else v_line(graphics, x, y_to, y_from, val);
}

static void draw_line(graphics_t* graphics, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t val) {
    const int16_t deltaX = abs(x2 - x1);
    const int16_t deltaY = abs(y2 - y1);
    const int16_t signX = x1 < x2 ? 1 : -1;
    const int16_t signY = y1 < y2 ? 1 : -1;
    int16_t error = deltaX - deltaY;
    set_pixel(graphics, x2, y2, val);
    while (x1 != x2 || y1 != y2) {
        set_pixel(graphics, x1, y1, val);
        const int16_t error2 = error * 2;
        if (error2 > -deltaY) {
            error -= deltaY;
            x1 += signX;
        }
        if (error2 < deltaX) {
            error += deltaX;
            y1 += signY;
        }
    }
}

static void graphics_clear(graphics_t* graphics) {
    memset(graphics->buffer, 0x00, WIDTH * HEIGHT * GRAY);
}

#endif	/* VFD_GRAPHICS_H */

