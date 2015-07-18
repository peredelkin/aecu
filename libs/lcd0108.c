#include "lcd0108.h"

//Функция управления портом данных
static void lcd_port_io(uint8_t io) {
    switch(io) {
        case 0: LCD_PORT = 0; LCD_DDR = 0; break;
        case 1: LCD_DDR = 0xFF; break;
    }
}

//Функция тактирования дисплея 
static void lcd0108_e_pin(lcd128x64_t* lcd,uint8_t state) {
    _delay_us(DELAY);
    switch(state) {
        case 0: pin_off(&lcd->e_p); break;
        case 1: pin_on(&lcd->e_p); break;
    }
    _delay_us(DELAY);
}

//Функция выбора микросхемы
static void lcd0108_cs_select(lcd128x64_t* lcd,uint8_t cs) {
    if(*lcd->cs_old != cs) { 
            switch(cs) {
            case 0: pin_off(&lcd->cs1_p); pin_on(&lcd->cs2_p); break;
            case 1: pin_on(&lcd->cs1_p); pin_off(&lcd->cs2_p); break;
            case 2: pin_off(&lcd->cs1_p); pin_off(&lcd->cs2_p); break;
        }
    *lcd->cs_old=cs;
    }
}

//Функция чтения флага занятости
static void lcd0108_read_wait_state(lcd128x64_t* lcd) {
    lcd_port_io(0);
    pin_off(&lcd->di_p);
    pin_on(&lcd->rw_p);
    lcd0108_e_pin(lcd,1);
    while((LCD_PIN & 0x80) == 0x80);
}

//Функция записи
static void lcd0108_write(lcd128x64_t* lcd,uint8_t data,uint8_t cs) {
    lcd0108_cs_select(lcd,cs);
    lcd0108_e_pin(lcd,1);
    lcd_port_io(1);
    LCD_PORT = data;
    lcd0108_e_pin(lcd,0);
}

//Функция записи данных/команд
static void lcd0108_write_di(lcd128x64_t* lcd,uint8_t data,uint8_t sel,uint8_t cs) {
    lcd0108_read_wait_state(lcd);
    lcd0108_e_pin(lcd,0);
    pin_off(&lcd->rw_p);
    switch(sel) {
        case 0: pin_off(&lcd->di_p); break;
        case 1: pin_on(&lcd->di_p); break;
    }
    lcd0108_write(lcd,data,cs);
}

//Функция выбора страницы
static void lcd0108_line_select(lcd128x64_t* lcd,uint8_t line) {
    lcd0108_write_di(lcd,0xb8 | line, 0, 2);
}

//Функция выбора колонны
static void lcd0108_column_select(lcd128x64_t* lcd,uint8_t column,uint8_t cs) {
    lcd0108_write_di(lcd,0x40 | column, 0, cs);
}

//Функция записи команды
static void lcd0108_write_cmd(lcd128x64_t* lcd,uint8_t cmd) {
    lcd0108_write_di(lcd,cmd, 0, 2);
}

//Функция записи одной линии
static void lcd0108_write_line(lcd128x64_t* lcd,uint8_t* data,uint8_t size) {
    uint8_t column = 0;
    uint8_t cs = 0;
    while (column < size) {
        if (column == 64) cs = 1;
        lcd0108_write_di(lcd,data[column], 1, cs);
        ++column;
    }
}

//Функция записи всего дисплея
void lcd0108_write_full(lcd128x64_t* lcd,uint8_t* data) {
    uint8_t i;
    for (i = 0; i < 8; ++i) {
        lcd0108_line_select(lcd,i);
        lcd0108_write_line(lcd,data + i * 128, 128);
    }
}

void lcd0108_write_selected(lcd128x64_t* lcd, uint8_t* data,uint8_t from,uint8_t to) {
    uint8_t i;
    for (i = from; i < to+1; ++i) {
        lcd0108_line_select(lcd,i);
        lcd0108_write_line(lcd,data + i * 128, 128);
    }
}

static void lcd0108_write_line_pgm(lcd128x64_t* lcd,const uint8_t* data, uint8_t size) {
    uint8_t column = 0;
    uint8_t cs = 0;
    while (column < size) {
        if (column == 64) cs = 1;
        lcd0108_write_di(lcd,pgm_read_byte(&data[column]), 1, cs);
        ++column;
    }
}

void lcd0108_write_full_pgm(lcd128x64_t* lcd,const uint8_t* data) {
    uint16_t i;
    for (i = 0; i < 8; ++i) {
        lcd0108_line_select(lcd,i);
        lcd0108_write_line_pgm(lcd,data + i * 128, 128);
    }
}

//Инициализация управления дисплеем
void lcd0108_init(lcd128x64_t* lcd) {
    pin_out(&lcd->e_p);
    pin_out(&lcd->rw_p);
    pin_out(&lcd->di_p);
    pin_out(&lcd->cs1_p);
    pin_out(&lcd->cs2_p);
    lcd0108_write_cmd(lcd,0x3F);
    lcd0108_write_cmd(lcd,0xC0);
    lcd0108_write_cmd(lcd,0x40);
}
