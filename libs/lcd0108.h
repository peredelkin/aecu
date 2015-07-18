/* 
 * File:   lcd0108.h
 * Author: ruslan
 *
 * Created on 1 Октябрь 2014 г., 8:14
 */

#ifndef LCD0108_H
#define	LCD0108_H

#include "ports.h"
#include <util/delay.h>
#include <avr/pgmspace.h>

#define LCD0108_WIDTH 128
#define LCD0108_HEIGHT 64
#define DISPLAY_BUFFER_SIZE (LCD0108_WIDTH * LCD0108_HEIGHT / 8)

#define DELAY  0.01
#define LCD_PIN PINA
#define LCD_PORT PORTA
#define LCD_DDR DDRA

typedef struct {
    pin_t e_p;
    pin_t rw_p;
    pin_t di_p;
    pin_t cs1_p;
    pin_t cs2_p;
    uint8_t* cs_old;
//    port_t lcd_pin;
//    port_t lcd_port;
//    port_t lcd_ddr;
} lcd128x64_t;

extern void lcd0108_init(lcd128x64_t* lcd);
extern void lcd0108_write_full(lcd128x64_t* lcd,uint8_t* data);
extern void lcd0108_write_selected(lcd128x64_t* lcd, uint8_t* data,uint8_t from,uint8_t to);
extern void lcd0108_write_full_pgm(lcd128x64_t* lcd,const uint8_t* data);


#endif	/* LCD0108_H */

/* main.c
 #include <avr/io.h>
#include <util/delay.h>
#include "libs/ports.h"
#include "libs/lcd0108.h"
#include <avr/pgmspace.h>
#include "libs/graphics.h"
#include "libs/font.h"
#include "libs/font_5x8_1251.h"
#include "libs/logo_niva.h"

font_bitmap_t f5x8_bmp = {
    32,255,FONT_5X8_1251_WIDTH, font_5x8_1251_data
};

font_t font5x8;

void init_font(void) {
    font_init(&font5x8, &f5x8_bmp, 1, 1, 5, 8, 1, 1);
}

//! Растр.
static graphics_t graphics;
//! Размер видеобуфера.
#define LCD0108_WIDTH 128
#define LCD0108_HEIGHT 64
#define DISPLAY_BUFFER_SIZE (LCD0108_WIDTH * LCD0108_HEIGHT / 8)
//! Видеобуфер.
static uint8_t display_buffer[DISPLAY_BUFFER_SIZE];

//==================================================================

lcd128x64_t lcd = {
    make_pin(B,0),
    make_pin(B,1),
    make_pin(B,2),
    make_pin(B,3),
    make_pin(B,4),
};

int main() {
    init_font();
    lcd0108_init(&lcd);
    graphics_init(&graphics, display_buffer, LCD0108_WIDTH, LCD0108_HEIGHT, GRAPHICS_BYTE_ALIGN_VERTICAL);
    graphics_clear(&graphics);
    graphics_set_pixel_value(&graphics, PIXEL_ON);
    graphics_set_fill_mode(&graphics, FILL_MODE_SOLID);
    graphics_set_paint_mode(&graphics,1);
    font_puts(&font5x8,&graphics,0,0,"Atmega 64");
    lcd0108_write_full(&lcd,display_buffer);
    _delay_ms(100);
    graphics_clear(&graphics);
    uint8_t a=32,b=0,c=0,d=0;
    DDRC|=0x01;
    while(1) {
            graphics_copy_image_pgm(&graphics, 0, 0, logo_niva_data, GRAPHICS_BYTE_ALIGN_VERTICAL, LOGO_NIVA_WIDTH, LOGO_NIVA_HEIGHT, LOGO_NIVA_WIDTH);
            graphics_circle(&graphics,a,b,10);
            lcd0108_write_full(&lcd,display_buffer);
            graphics_clear(&graphics);
            switch(c) {
                case 0: a++; break;
                case 1: a--; break;
            }
            switch(d) {
                case 0: b++; break;
                case 1: b--; break;
            }
            PORTC^=0x01;
            if(b==64) d=1;
            if(b==0) d=0;
            if(a==128) c=1;
            if(a==0) c=0;
    }
}
 */

/* Арканоид
#include <avr/io.h>
#include <util/delay.h>
#include "libs/ports.h"
#include "libs/lcd0108.h"
#include <avr/pgmspace.h>
#include "libs/graphics.h"
#include "libs/graph.h"
#include "libs/font.h"
#include "libs/font_5x8_1251.h"
#include <stdio.h>
#include "libs/pin_state.h"

font_bitmap_t f5x8_bmp = {
    32,255,FONT_5X8_1251_WIDTH, font_5x8_1251_data
};

graph_t graphy;
graph_t graphx;

font_t font5x8;

void init_font(void) {
    font_init(&font5x8, &f5x8_bmp, 1, 1, 5, 8, 1, 1);
}

//! Растр.
static graphics_t graphics;
//! Размер видеобуфера.
#define LCD0108_WIDTH 128
#define LCD0108_HEIGHT 64
#define DISPLAY_BUFFER_SIZE (LCD0108_WIDTH * LCD0108_HEIGHT / 8)
//! Видеобуфер.
static uint8_t display_buffer[DISPLAY_BUFFER_SIZE];

//==================================================================

lcd128x64_t lcd = {
    make_pin(C,0),
    make_pin(C,1),
    make_pin(C,2),
    make_pin(C,3),
    make_pin(C,4),
};

//структура блоков
typedef struct {
    int8_t x;
    int8_t y;
    uint8_t width; //ширина
    uint8_t height; //высота
    uint8_t draw;
} block_t;

block_t block[] = {
    {2, 2, 6, 3, 1},
    {10, 2, 6, 3, 1},
    {18, 2, 6, 3, 1},
    {26, 2, 6, 3, 1},
    {34, 2, 6, 3, 1},
    {42, 2, 6, 3, 1},
    {50, 2, 6, 3, 1},
    {58, 2, 6, 3, 1},
    {2, 7, 6, 3, 1},
    {10, 7, 6, 3, 1},
    {18, 7, 6, 3, 1},
    {26, 7, 6, 3, 1},
    {34, 7, 6, 3, 1},
    {42, 7, 6, 3, 1},
    {50, 7, 6, 3, 1},
    {58, 7, 6, 3, 1},
    {2, 12, 6, 3, 1},
    {10, 12, 6, 3, 1},
    {18, 12, 6, 3, 1},
    {26, 12, 6, 3, 1},
    {34, 12, 6, 3, 1},
    {42, 12, 6, 3, 1},
    {50, 12, 6, 3, 1},
    {58, 12, 6, 3, 1},
    {2, 17, 6, 3, 1},
    {10, 17, 6, 3, 1},
    {18, 17, 6, 3, 1},
    {26, 17, 6, 3, 1},
    {34, 17, 6, 3, 1},
    {42, 17, 6, 3, 1},
    {50, 17, 6, 3, 1},
    {58, 17, 6, 3, 1},
    {2, 22, 6, 3, 1},
    {10, 22, 6, 3, 1},
    {18, 22, 6, 3, 1},
    {26, 22, 6, 3, 1},
    {34, 22, 6, 3, 1},
    {42, 22, 6, 3, 1},
    {50, 22, 6, 3, 1},
    {58, 22, 6, 3, 1},
    {2, 27, 6, 3, 1},
    {10, 27, 6, 3, 1},
    {18, 27, 6, 3, 1},
    {26, 27, 6, 3, 1},
    {34, 27, 6, 3, 1},
    {42, 27, 6, 3, 1},
    {50, 27, 6, 3, 1},
    {58, 27, 6, 3, 1},
    {2, 32, 6, 3, 1},
    {10, 32, 6, 3, 1},
    {18, 32, 6, 3, 1},
    {26, 32, 6, 3, 1},
    {34, 32, 6, 3, 1},
    {42, 32, 6, 3, 1},
    {50, 32, 6, 3, 1},
    {58, 32, 6, 3, 1},
    {2, 37, 6, 3, 1},
    {10, 37, 6, 3, 1},
    {18, 37, 6, 3, 1},
    {26, 37, 6, 3, 1},
    {34, 37, 6, 3, 1},
    {42, 37, 6, 3, 1},
    {50, 37, 6, 3, 1},
    {58, 37, 6, 3, 1}
};

//структура мяча
typedef struct {
    int8_t x;
    int8_t y;
    uint8_t rad;
    uint8_t x_vect;
    uint8_t y_vect;
} ball_t;

ball_t ball = {
    32,56,2,0,0
};

typedef struct {
    int8_t x;
    int8_t y;
    uint8_t width;
} club_t;

club_t club = {
    25,60,15
};

void move_club(club_t* club,pin_t* pin_1,pin_t* pin_2) {
    graphics_hline(&graphics,club->y,club->x,club->x+club->width);
    if(button_state(pin_1,0,0) && ((club->x+club->width)<=64)) club->x+=2;
    if(button_state(pin_2,0,0) && club->x>=2) club->x-=2;
}

void ball_club_collision(ball_t* ball,club_t* club) {
    if(ball->y == club->y-ball->rad) {
        if(ball->x >= club->x-ball->rad-1 &&
           ball->x <= club->x+4) {
            ball->x_vect=0;
            ball->y_vect=0;
        }
        if(ball->x >= club->x+5 &&
           ball->x <= club->x+9) {
            ball->y_vect=0;
        }
        if(ball->x >= club->x+10 &&
           ball->x <= club->x+15+ball->rad+1)
        {
            ball->y_vect=0;
            ball->x_vect=1;
        }
    }
}

void move_ball(ball_t* ball) {
    graphics_set_fill_mode(&graphics, FILL_MODE_SOLID);
    graphics_circle(&graphics, ball->x, ball->y, ball->rad);
    graphics_set_fill_mode(&graphics, FILL_MODE_NONE);
    if(ball->x_vect) ++ball->x; else --ball->x;
    if(ball->y_vect) ++ball->y; else --ball->y;
}

void draw_block(block_t * block) {

    if (block->draw) {
        graphics_square
                (&graphics, block->x, block->y, block->x + block->width, block->y + block->height);
    }
}

void draw_mass_block(block_t * block, uint8_t size) {
    uint8_t i = 0;
    while (i < size) {
        draw_block(&block[i]);
        ++i;
    }
}

void ball_block_collision(ball_t* ball, block_t* block) {
    if (block->draw) {
        if (ball->x >= block->x &&
                ball->x <= block->x + block->width) {
            //попадание по нижней части блока
            if (ball->y <= block->y + block->height + ball->rad &&
                    ball->y >= block->y + block->height) {
                ball->y_vect = 1;
                block->draw = 0;
            }
            //попадание по верхней части блока
            if (ball->y >= block->y - ball->rad &&
                    ball->y <= block->y) {
                ball->y_vect = 0;
                block->draw = 0;
            }
        }
        if (ball->y >= block->y&&
                ball->y <= block->y + block->height) {
            //попадание правой части блока
            if (ball->x <= block->x + block->width + ball->rad &&
                    ball->x >= block->x + block->width) {
                ball->x_vect = 1;
                block->draw = 0;
            }
            //попадание по левой части блока
            if (ball->x >= block->x - ball->rad &&
                    ball->x <= block->x) {
                ball->x_vect = 0;
                block->draw = 0;
            }
        }
    }
    if (ball->x <= ball->rad) ball->x_vect = 1;
    if (ball->x >= 66 - ball->rad) ball->x_vect = 0;
    if (ball->y <= ball->rad) ball->y_vect = 1;
    if (ball->y >= 64) ball->y_vect = 0;
    //while(1) {
//font_puts(&font5x8, &graphics, 10, 32, "GAME OVER");
//lcd0108_write_full(&lcd, display_buffer);
//graphics_clear(&graphics);
//    }
}

void ball_block_mass_collision(ball_t* ball,block_t* block,uint8_t size) {
    uint8_t i = 0;
    while (i < size) {
        ball_block_collision(ball,&block[i]);
        ++i;
    }
}

char buf[32]; //буфер snprintf

graph_t points;
graph_t points_2;

pin_t g1 = make_pin(G, 0);
pin_t g2 = make_pin(G, 1);

void draw_ball_graph (graphics_t* graphics ,graph_t* graph,ball_t* ball,club_t* club,uint8_t from,uint8_t to) {
    draw_ygraph(graph,graphics,from,to,ball->x);
    draw_ygraph(graph,graphics,from,to,club->x+club->width/2);
}

int main() {
    init_ygraph(&graphy,32);
    init_ygraph(&graphx,32);
    
    pin_in_pu(&g1);
    pin_in_pu(&g2);
    
    init_font();
    
    lcd0108_init(&lcd);
    
    init_ygraph(&points,32);
    init_xgraph(&points_2,96);
    
    graphics_init(&graphics, display_buffer, LCD0108_WIDTH, LCD0108_HEIGHT, GRAPHICS_BYTE_ALIGN_VERTICAL);
    graphics_clear(&graphics);
    
    graphics_set_pixel_value(&graphics, PIXEL_ON);
    graphics_set_fill_mode(&graphics, FILL_MODE_NONE);
    graphics_set_paint_mode(&graphics, PAINT_MODE_SET);
    
    _delay_ms(100);
    
    font_puts(&font5x8, &graphics, 0, 0, "Atmega 64");
    font_puts(&font5x8, &graphics, 0, 10, "Arkanoid(alpha)");
    lcd0108_write_full(&lcd, display_buffer);
    
    _delay_ms(2000);
    
    graphics_clear(&graphics);
    
    while (1) {
        //draw_ball_graph(&graphics,&graphy,&ball,&club,67,128);
        draw_mass_block(block,64);
        move_ball(&ball);
        move_club(&club,&g1,&g2);
        ball_block_mass_collision(&ball,block,64);
        ball_club_collision(&ball,&club);
        
        graphics_hline(&graphics,0,0,66);
        graphics_vline(&graphics,0,0,63);
        graphics_vline(&graphics,66,0,63);
        
        //snprintf(buf, 64, "X %d", a);
        //font_puts(&font5x8, &graphics, 0, 0, buf);
        //snprintf(buf, 64, "Y %d", b);
        //font_puts(&font5x8, &graphics, 0, 8, buf);
        
        lcd0108_write_full(&lcd, display_buffer);
        graphics_clear(&graphics);
    }
}
 */