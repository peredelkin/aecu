#include "vfd_graphics.h"

/*
void set_pixel(graphics_t* graphics, int16_t x, int16_t y, uint8_t val) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT * 8) return;
    graphics->buffer[(16 * x)+(y / 4)] |= (val << 6 - (y * 2) % 8);
}

void h_line(graphics_t* graphics, int16_t x_from, int16_t x_to, int16_t y, uint8_t val) {
    int16_t x_n = x_to + 1;
    while (x_from != x_n) {
        x_n--;
        set_pixel(graphics, x_n, y, val);
    }
}

void draw_h_line(graphics_t* graphics, int16_t x_from, int16_t x_to, int16_t y, uint8_t val) {
    if(x_from <= x_to) h_line(graphics,x_from,x_to,y,val);
    else h_line(graphics,x_to,x_from,y,val);
}

void v_line(graphics_t* graphics, int16_t x, int16_t y_from, int16_t y_to, uint8_t val) {
    int16_t y_n = y_to + 1;
    while (y_from != y_n) {
        y_n--;
        set_pixel(graphics, x, y_n, val);
    }
}

void draw_v_line(graphics_t* graphics, int16_t x, int16_t y_from, int16_t y_to, uint8_t val) {
    if(y_from <= y_to) v_line(graphics,x,y_from,y_to,val);
    else v_line(graphics,x,y_to,y_from,val);
}

void graphics_clear(graphics_t* graphics) {
    memset(graphics->buffer, 0x0, WIDTH*HEIGHT*GRAY);
}
*/
